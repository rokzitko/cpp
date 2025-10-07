#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <sstream>
#include <algorithm>

using boost::asio::ip::tcp;

// Small helper: trim whitespace
inline std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Type for SCPI command handlers
using CommandHandler = std::function<std::string(const std::string&)>;

class ScpiSession : public std::enable_shared_from_this<ScpiSession> {
public:
    explicit ScpiSession(tcp::socket socket)
        : socket_(std::move(socket)) 
    {
        register_commands();
    }

    void start() { read(); }

private:
    tcp::socket socket_;
    boost::asio::streambuf buffer_;

    // Example internal state
    double voltage_ = 1.0;

    // Command registry
    std::unordered_map<std::string, CommandHandler> handlers_;

    void register_commands() {
        // Common commands
        handlers_["*IDN?"] = [this](const std::string&) {
            return "MyCompany,SCPI-Stub,1234,0.1";
        };
        handlers_["*RST"] = [this](const std::string&) {
            voltage_ = 1.0;
            return "";
        };
        handlers_["*CLS"] = [this](const std::string&) {
            // Clear status registers (stub)
            return "";
        };
        handlers_["*TST?"] = [](const std::string&) {
            return "0"; // self-test OK
        };

        // Source voltage
        handlers_[":SOUR:VOLT?"] = [this](const std::string&) {
            return std::to_string(voltage_);
        };
        handlers_[":SOUR:VOLT"] = [this](const std::string& arg) {
            try {
                voltage_ = std::stod(arg);
            } catch (...) {}
            return "";
        };

        // Measurement
        handlers_[":MEAS:VOLT?"] = [this](const std::string&) {
            return std::to_string(voltage_ + 0.001);
        };
    }

    void read() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, buffer_, '\n',
            [this, self](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    std::istream is(&buffer_);
                    std::string line;
                    std::getline(is, line);
                    if (!line.empty() && line.back() == '\r') line.pop_back();
                    line = trim(line);
                    if (!line.empty()) handle_command(line);
                    read();
                }
            });
    }

    void write(const std::string &msg) {
        if (msg.empty()) return;
        auto self(shared_from_this());
        auto out = std::make_shared<std::string>(msg + "\n");
        boost::asio::async_write(socket_, boost::asio::buffer(*out),
            [this, self, out](boost::system::error_code, std::size_t) {});
    }

    void handle_command(const std::string &line) {
        // Uppercase for matching (except numbers/args)
        std::string cmd = line;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(),
                       [](unsigned char c){ return std::toupper(c); });

        // Separate keyword and argument
        std::string keyword = cmd;
        std::string arg;
        auto pos = line.find(' ');
        if (pos != std::string::npos) {
            keyword = cmd.substr(0, pos);
            arg = trim(line.substr(pos+1));
        }

        // Try exact match
        auto it = handlers_.find(keyword);
        if (it != handlers_.end()) {
            std::string resp = it->second(arg);
            if (!resp.empty()) write(resp);
            return;
        }

        // Try query form (with '?')
        if (keyword.back() == '?') {
            it = handlers_.find(keyword);
            if (it != handlers_.end()) {
                std::string resp = it->second(arg);
                if (!resp.empty()) write(resp);
                return;
            }
        }

        // Unknown
        write("ERROR");
    }
};

class ScpiServer {
public:
    ScpiServer(boost::asio::io_context &io, unsigned short port)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
        accept();
    }

private:
    tcp::acceptor acceptor_;

    void accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<ScpiSession>(std::move(socket))->start();
                }
                accept();
            });
    }
};

int main() {
    try {
        boost::asio::io_context io;
        ScpiServer server(io, 5025);
        std::cout << "SCPI server listening on port 5025...\n";
        io.run();
    } catch (std::exception &e) {
        std::cerr << "Fatal: " << e.what() << "\n";
    }
}

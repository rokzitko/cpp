#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <memory>

using boost::asio::ip::tcp;

class ScpiSession : public std::enable_shared_from_this<ScpiSession> {
public:
    explicit ScpiSession(tcp::socket socket)
        : socket_(std::move(socket)) {}

    void start() { read(); }

private:
    tcp::socket socket_;
    boost::asio::streambuf buffer_;

    // Example internal state
    double voltage_ = 1.0;

    void read() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, buffer_, '\n',
            [this, self](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    std::istream is(&buffer_);
                    std::string line;
                    std::getline(is, line);
                    if (!line.empty() && line.back() == '\r') line.pop_back();
                    handle_command(line);
                    read();
                }
            });
    }

    void write(const std::string &msg) {
        auto self(shared_from_this());
        auto out = std::make_shared<std::string>(msg + "\n");
        boost::asio::async_write(socket_, boost::asio::buffer(*out),
            [this, self, out](boost::system::error_code, std::size_t) {});
    }

    void handle_command(const std::string &cmd_in) {
        std::string cmd = cmd_in;
        for (auto &c : cmd) c = std::toupper(c);

        if (cmd == "*IDN?") {
            write("MyCompany,SCPI-Stub,1234,0.1");
        }
        else if (cmd == "*RST") {
            voltage_ = 1.0;
        }
        else if (cmd == "*CLS") {
            // clear status registers (stub)
        }
        else if (cmd == "*TST?") {
            write("0"); // self-test OK
        }
        else if (cmd.rfind(":SOUR:VOLT", 0) == 0) {
            // Example: ":SOUR:VOLT 3.3"
            if (cmd.find('?') != std::string::npos) {
                write(std::to_string(voltage_));
            } else {
                double v;
                if (parse_double(cmd_in, v)) {
                    voltage_ = v;
                }
            }
        }
        else if (cmd.rfind(":MEAS:VOLT?", 0) == 0) {
            // Return a fake measurement
            write(std::to_string(voltage_ + 0.001)); 
        }
        else {
            // Unknown command
            write("ERROR");
        }
    }

    static bool parse_double(const std::string &cmd, double &out) {
        std::istringstream iss(cmd);
        std::string keyword;
        iss >> keyword;
        return (iss >> out) ? true : false;
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


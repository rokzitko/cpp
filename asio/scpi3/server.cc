#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <sstream>
#include <algorithm>
#include <deque>

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

// === Status system definitions ===

// Standard Event Status Register bits (IEEE 488.2)
enum SesrBits : uint8_t {
    OPERATION_COMPLETE = 1 << 0,
    QUERY_ERROR        = 1 << 2,
    DEVICE_ERROR       = 1 << 3,
    EXECUTION_ERROR    = 1 << 4,
    COMMAND_ERROR      = 1 << 5,
    USER_REQUEST       = 1 << 6,
    POWER_ON           = 1 << 7
};

// Status Byte bits
enum StbBits : uint8_t {
    EVENT_STATUS_BIT   = 1 << 5, // summary of SESR
    MESSAGE_AVAILABLE  = 1 << 4  // output queue not empty
};

class ScpiSession : public std::enable_shared_from_this<ScpiSession> {
public:
    explicit ScpiSession(tcp::socket socket)
        : socket_(std::move(socket)) 
    {
        register_commands();
        sesr_ |= POWER_ON; // set power-on event
    }

    void start() { read(); }

private:
    tcp::socket socket_;
    boost::asio::streambuf buffer_;

    // Example internal state
    double voltage_ = 1.0;

    // Command registry
    std::unordered_map<std::string, CommandHandler> handlers_;

    // === Status system state ===
    std::deque<std::string> error_queue_; // instrument error queue
    uint8_t sesr_ = 0;                    // Standard Event Status Register
    uint8_t ese_mask_ = 0;                // Enable mask for SESR
    uint8_t sre_mask_ = 0;                // Service Request Enable (not fully used)
    uint8_t stb_ = 0;                     // Status Byte register (summary)

    void push_error(const std::string& err, SesrBits bit) {
        if (error_queue_.size() > 10) error_queue_.pop_front();
        error_queue_.push_back(err);
        sesr_ |= bit;
    }

    void clear_status() {
        error_queue_.clear();
        sesr_ = 0;
        stb_ = 0;
    }

    void register_commands() {
        // --- Common mandatory commands ---
        handlers_["*IDN?"] = [this](const std::string&) {
            return "MyCompany,SCPI-Stub,1234,0.2";
        };
        handlers_["*RST"] = [this](const std::string&) {
            voltage_ = 1.0;
            return "";
        };
        handlers_["*CLS"] = [this](const std::string&) {
            clear_status();
            return "";
        };
        handlers_["*TST?"] = [](const std::string&) {
            return "0"; // self-test OK
        };

        // --- Status system commands ---
        handlers_["*ESR?"] = [this](const std::string&) {
            uint8_t val = sesr_;
            sesr_ = 0; // reading clears
            return std::to_string(val);
        };
        handlers_["*ESE"] = [this](const std::string& arg) {
            try { ese_mask_ = static_cast<uint8_t>(std::stoi(arg)); } catch (...) {}
            return "";
        };
        handlers_["*ESE?"] = [this](const std::string&) {
            return std::to_string(ese_mask_);
        };
        handlers_["*STB?"] = [this](const std::string&) {
            // recompute STB summary
            stb_ = 0;
            if (sesr_ & ese_mask_) stb_ |= EVENT_STATUS_BIT;
            if (!error_queue_.empty()) stb_ |= MESSAGE_AVAILABLE;
            return std::to_string(stb_);
        };
        handlers_["*SRE"] = [this](const std::string& arg) {
            try { sre_mask_ = static_cast<uint8_t>(std::stoi(arg)); } catch (...) {}
            return "";
        };
        handlers_["*SRE?"] = [this](const std::string&) {
            return std::to_string(sre_mask_);
        };

        // --- Error query ---
        handlers_["SYST:ERR?"] = [this](const std::string&) {
            if (error_queue_.empty()) return std::string("0, \"No error\"");
            auto err = error_queue_.front();
            error_queue_.pop_front();
            return "100, \"" + err + "\""; // stub error code 100
        };

        // --- Application commands ---
        handlers_[":SOUR:VOLT?"] = [this](const std::string&) {
            return std::to_string(voltage_);
        };
        handlers_[":SOUR:VOLT"] = [this](const std::string& arg) {
            try {
                voltage_ = std::stod(arg);
            } catch (...) {
                push_error("Execution error: bad voltage", EXECUTION_ERROR);
            }
            return "";
        };
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
        // Uppercase for matching (keywords only)
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

        auto it = handlers_.find(keyword);
        if (it != handlers_.end()) {
            std::string resp = it->second(arg);
            if (!resp.empty()) write(resp);
        } else {
            push_error("Command error: unknown command '" + line + "'", COMMAND_ERROR);
            write("ERROR");
        }
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
        std::cout << "SCPI server with status system listening on port 5025...\n";
        io.run();
    } catch (std::exception &e) {
        std::cerr << "Fatal: " << e.what() << "\n";
    }
}

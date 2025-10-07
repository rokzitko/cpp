#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <algorithm>
#include <deque>
#include <map>
#include <vector>
#include <functional>

using boost::asio::ip::tcp;

// --- Utilities ---
inline std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// --- SCPI Command Tree ---
struct ScpiNode {
    std::string name;       // full keyword (e.g., "VOLTage")
    size_t min_len;         // min abbreviation (e.g., 4)
    std::map<std::string, std::shared_ptr<ScpiNode>> children;

    // Handlers: set or query
    std::function<std::string(const std::string&)> set_handler;
    std::function<std::string()> query_handler;

    explicit ScpiNode(std::string n) : name(std::move(n)) {
        // min_len = number of uppercase letters
        min_len = 0;
        for (char c : name) if (std::isupper(c)) ++min_len;
        if (min_len == 0) min_len = name.size();
    }

    // Try to match a token against this node
    bool matches(const std::string& token) const {
        if (token.size() < min_len) return false;
        std::string upname = name;
        std::transform(upname.begin(), upname.end(), upname.begin(), ::toupper);

        std::string uptok = token;
        std::transform(uptok.begin(), uptok.end(), uptok.begin(), ::toupper);

        return upname.compare(0, uptok.size(), uptok) == 0;
    }
};

// --- Status bits (same as before) ---
enum SesrBits : uint8_t {
    OPERATION_COMPLETE = 1 << 0,
    QUERY_ERROR        = 1 << 2,
    DEVICE_ERROR       = 1 << 3,
    EXECUTION_ERROR    = 1 << 4,
    COMMAND_ERROR      = 1 << 5,
    USER_REQUEST       = 1 << 6,
    POWER_ON           = 1 << 7
};
enum StbBits : uint8_t {
    EVENT_STATUS_BIT   = 1 << 5,
    MESSAGE_AVAILABLE  = 1 << 4
};

// --- Session class ---
class ScpiSession : public std::enable_shared_from_this<ScpiSession> {
public:
    explicit ScpiSession(tcp::socket socket)
        : socket_(std::move(socket)) {
        build_tree();
        sesr_ |= POWER_ON;
    }

    void start() { read(); }

private:
    tcp::socket socket_;
    boost::asio::streambuf buffer_;

    // State
    double voltage_ = 1.0;
    std::deque<std::string> error_queue_;
    uint8_t sesr_ = 0;
    uint8_t ese_mask_ = 0;
    uint8_t sre_mask_ = 0;
    uint8_t stb_ = 0;

    std::shared_ptr<ScpiNode> root_;

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

    // --- Build SCPI command hierarchy ---
    void build_tree() {
        root_ = std::make_shared<ScpiNode>("ROOT");

        // * commands (common)
        auto idn = std::make_shared<ScpiNode>("*IDN");
        idn->query_handler = [this]() { return "MyCompany,SCPI-Stub,1234,0.4"; };
        root_->children["*IDN"] = idn;

        auto rst = std::make_shared<ScpiNode>("*RST");
        rst->set_handler = [this](const std::string&) { voltage_ = 1.0; return ""; };
        root_->children["*RST"] = rst;

        auto cls = std::make_shared<ScpiNode>("*CLS");
        cls->set_handler = [this](const std::string&) { clear_status(); return ""; };
        root_->children["*CLS"] = cls;

        auto opc = std::make_shared<ScpiNode>("*OPC");
        opc->set_handler = [this](const std::string&) { sesr_ |= OPERATION_COMPLETE; return ""; };
        opc->query_handler = []() { return "1"; };
        root_->children["*OPC"] = opc;

        auto esr = std::make_shared<ScpiNode>("*ESR");
        esr->query_handler = [this]() { uint8_t v = sesr_; sesr_ = 0; return std::to_string(v); };
        root_->children["*ESR"] = esr;

        // Status byte
        auto stb = std::make_shared<ScpiNode>("*STB");
        stb->query_handler = [this]() {
            stb_ = 0;
            if (sesr_ & ese_mask_) stb_ |= EVENT_STATUS_BIT;
            if (!error_queue_.empty()) stb_ |= MESSAGE_AVAILABLE;
            return std::to_string(stb_);
        };
        root_->children["*STB"] = stb;

        // Error query
        auto err = std::make_shared<ScpiNode>("SYSTem:ERRor");
        err->query_handler = [this]() {
            if (error_queue_.empty()) return std::string("0, \"No error\"");
            auto e = error_queue_.front(); error_queue_.pop_front();
            return "100, \"" + e + "\"";
        };
        root_->children["SYST:ERR"] = err;

        // Source/Measure hierarchy
        auto sour = std::make_shared<ScpiNode>("SOURce");
        auto volt = std::make_shared<ScpiNode>("VOLTage");
        volt->set_handler = [this](const std::string& arg) {
            try { voltage_ = std::stod(arg); }
            catch (...) { push_error("Execution error: bad voltage", EXECUTION_ERROR); }
            return "";
        };
        volt->query_handler = [this]() { return std::to_string(voltage_); };
        sour->children["VOLT"] = volt;
        root_->children["SOUR"] = sour;

        auto meas = std::make_shared<ScpiNode>("MEASure");
        auto mvolt = std::make_shared<ScpiNode>("VOLTage");
        mvolt->query_handler = [this]() { return std::to_string(voltage_ + 0.001); };
        meas->children["VOLT"] = mvolt;
        root_->children["MEAS"] = meas;
    }

    // --- Parser ---
    void handle_command(const std::string &line) {
        std::string cmd = line;
        if (cmd.empty()) return;

        // Split into tokens by colon
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream iss(cmd);
        while (std::getline(iss, token, ':')) {
            if (!token.empty()) tokens.push_back(trim(token));
        }

        // Query?
        bool is_query = false;
        if (!tokens.empty()) {
            if (!tokens.back().empty() && tokens.back().back() == '?') {
                is_query = true;
                tokens.back().pop_back();
            }
        }

        // Argument?
        std::string arg;
        if (!tokens.empty()) {
            auto pos = tokens.back().find(' ');
            if (pos != std::string::npos) {
                arg = trim(tokens.back().substr(pos+1));
                tokens.back() = tokens.back().substr(0, pos);
            }
        }

        // Walk tree
        auto node = root_;
        for (const auto& t : tokens) {
            bool matched = false;
            for (auto &kv : node->children) {
                if (kv.second->matches(t)) {
                    node = kv.second;
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                push_error("Command error: unknown token '" + t + "'", COMMAND_ERROR);
                write("ERROR");
                return;
            }
        }

        // Execute
        std::string resp;
        if (is_query) {
            if (node->query_handler) resp = node->query_handler();
            else push_error("Query error: not queryable", QUERY_ERROR);
        } else {
            if (node->set_handler) resp = node->set_handler(arg);
            else if (node->query_handler) push_error("Query form required", QUERY_ERROR);
            else push_error("Command error: no action", COMMAND_ERROR);
        }
        if (!resp.empty()) write(resp);
    }

    // --- I/O ---
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
                    handle_command(line);
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
};

// --- Server ---
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
                if (!ec) std::make_shared<ScpiSession>(std::move(socket))->start();
                accept();
            });
    }
};

int main() {
    try {
        boost::asio::io_context io;
        ScpiServer server(io, 5025);
        std::cout << "SCPI server with parser listening on port 5025...\n";
        io.run();
    } catch (std::exception &e) {
        std::cerr << "Fatal: " << e.what() << "\n";
    }
}

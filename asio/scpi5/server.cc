#include "scpi_server.hpp"

// Example derived session
class MyScpiSession : public ScpiSessionBase {
public:
    explicit MyScpiSession(tcp::socket socket) : ScpiSessionBase(std::move(socket)) {
        build_tree();
    }
private:
    double voltage_ = 1.0;
    std::string trig_source_ = "IMM";
    std::string trig_slope_  = "POS";
    double trig_level_ = 0.0;
    double trig_delay_ = 0.0;
    bool measuring_ = false;

    void build_tree() {
        // *IDN?
        add_node({"*IDN"}, {}, [this]() { return "MyCompany,SCPI-Stub,1234,0.6"; });
        // *RST
        add_node({"*RST"}, [this](const std::string&) { voltage_ = 1.0; return ""; });
        // *CLS
        add_node({"*CLS"}, [this](const std::string&) { clear_status(); return ""; });
        // *OPC, *OPC?
        add_node({"*OPC"}, [this](const std::string&) { sesr_ |= OPERATION_COMPLETE; return ""; },
                              [](){ return "1"; });
        // *ESR?
        add_node({"*ESR"}, {}, [this]() { uint8_t v = sesr_; sesr_ = 0; return std::to_string(v); });
        // *STB?
        add_node({"*STB"}, {}, [this]() { 
            stb_ = 0;
            if (sesr_ & ese_mask_) stb_ |= EVENT_STATUS_BIT;
            if (!error_queue_.empty()) stb_ |= MESSAGE_AVAILABLE;
            return std::to_string(stb_);
        });
        // SYST:ERR?
        add_node({"SYSTem","ERRor"}, {}, [this]() {
            if (error_queue_.empty()) return std::string("0, \"No error\"");
            auto e = error_queue_.front(); error_queue_.pop_front();
            return "100, \"" + e + "\"";
        });

        // SOURce:VOLTage
        add_node({"SOURce","VOLTage"},
            [this](const std::string& arg) {
                try { voltage_ = std::stod(arg); }
                catch (...) { push_error("Execution error: bad voltage", EXECUTION_ERROR); }
                return "";
            },
            [this]() { return std::to_string(voltage_); });

        // MEASure:VOLTage?
        add_node({"MEASure","VOLTage"}, {}, [this]() {
            return std::to_string(voltage_ + 0.001);
        });

        // TRIGger:SOURce
        add_node({"TRIGger","SOURce"},
            [this](const std::string& arg) {
                std::string up = arg; for (auto& c: up) c = toupper(c);
                if (up=="IMM"||up=="EXT"||up=="BUS") trig_source_=up;
                else push_error("Execution error: bad trigger source", EXECUTION_ERROR);
                return "";
            },
            [this]() { return trig_source_; });

        // TRIGger:LEVel
        add_node({"TRIGger","LEVel"},
            [this](const std::string& arg) {
                try { trig_level_ = std::stod(arg); }
                catch (...) { push_error("Execution error: bad level", EXECUTION_ERROR); }
                return "";
            },
            [this]() { return std::to_string(trig_level_); });

        // TRIGger:SLOPe
        add_node({"TRIGger","SLOPe"},
            [this](const std::string& arg) {
                std::string up = arg; for (auto& c: up) c = toupper(c);
                if (up=="POS"||up=="NEG") trig_slope_=up;
                else push_error("Execution error: bad slope", EXECUTION_ERROR);
                return "";
            },
            [this]() { return trig_slope_; });

        // TRIGger:DELay
        add_node({"TRIGger","DELay"},
            [this](const std::string& arg) {
                try { trig_delay_ = std::stod(arg); }
                catch (...) { push_error("Execution error: bad delay", EXECUTION_ERROR); }
                return "";
            },
            [this]() { return std::to_string(trig_delay_); });

        // INITiate
        add_node({"INITiate"},
            [this](const std::string&) { measuring_=true; sesr_|=OPERATION_COMPLETE; return ""; });

        // ABORt
        add_node({"ABORt"},
            [this](const std::string&) { measuring_=false; return ""; });

        // *WAI
        add_node({"*WAI"}, [this](const std::string&) { return ""; });
    }
};

// Derived server
class MyScpiServer : public ScpiServerBase {
public:
    MyScpiServer(boost::asio::io_context &io, unsigned short port)
        : ScpiServerBase(io, port) {}
protected:
    std::shared_ptr<ScpiSessionBase> make_session(tcp::socket socket) override {
        return std::make_shared<MyScpiSession>(std::move(socket));
    }
};

// Entry point
int main() {
    try {
        boost::asio::io_context io;
        MyScpiServer server(io, 5025);
        std::cout << "My SCPI stub running on port 5025...\n";
        io.run();
    } catch (std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
    }
}

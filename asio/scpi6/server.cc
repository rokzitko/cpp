#include "scpi_server.hpp"

class MyScpiSession : public ScpiSessionBase {
public:
    explicit MyScpiSession(tcp::socket socket) : ScpiSessionBase(std::move(socket)) {
        load_commands(make_command_table());
    }
private:
    double voltage_ = 1.0;
    std::string trig_source_ = "IMM";
    std::string trig_slope_  = "POS";
    double trig_level_ = 0.0;
    double trig_delay_ = 0.0;
    bool measuring_ = false;

    std::vector<ScpiCommandDef> make_command_table() {
        return {
            // Common
            {{"*IDN"}, {}, [this]() { return "MyCompany,SCPI-Stub,1234,0.7"; }},
            {{"*RST"}, [this](const std::string&){ voltage_=1.0; return ""; }},
            {{"*CLS"}, [this](const std::string&){ clear_status(); return ""; }},
            {{"*OPC"}, [this](const std::string&){ sesr_ |= OPERATION_COMPLETE; return ""; },
                      [](){ return "1"; }},
            {{"*ESR"}, {}, [this](){ uint8_t v=sesr_; sesr_=0; return std::to_string(v);} },
            {{"*STB"}, {}, [this](){
                stb_=0;
                if (sesr_ & ese_mask_) stb_ |= EVENT_STATUS_BIT;
                if (!error_queue_.empty()) stb_ |= MESSAGE_AVAILABLE;
                return std::to_string(stb_);
            }},
            {{"*WAI"}, [this](const std::string&){ return ""; }},

            // System error
            {{"SYSTem","ERRor"}, {}, [this](){
                if (error_queue_.empty()) return std::string("0, \"No error\"");
                auto e = error_queue_.front(); error_queue_.pop_front();
                return "100, \"" + e + "\"";
            }},

            // Source/Measure
            {{"SOURce","VOLTage"},
                [this](const std::string& arg){
                    try{ voltage_=std::stod(arg);}
                    catch(...){ push_error("Execution error: bad voltage",EXECUTION_ERROR);}
                    return "";
                },
                [this](){ return std::to_string(voltage_);}
            },
            {{"MEASure","VOLTage"}, {}, [this](){ return std::to_string(voltage_+0.001);}},

            // Trigger subsystem
            {{"TRIGger","SOURce"},
                [this](const std::string& arg){
                    std::string up=arg; for(auto&c:up)c=toupper(c);
                    if(up=="IMM"||up=="EXT"||up=="BUS") trig_source_=up;
                    else push_error("Execution error: bad trigger source",EXECUTION_ERROR);
                    return "";
                },
                [this](){ return trig_source_; }},
            {{"TRIGger","LEVel"},
                [this](const std::string& arg){
                    try{ trig_level_=std::stod(arg);}
                    catch(...){ push_error("Execution error: bad level",EXECUTION_ERROR);}
                    return "";
                },
                [this](){ return std::to_string(trig_level_);} },
            {{"TRIGger","SLOPe"},
                [this](const std::string& arg){
                    std::string up=arg; for(auto&c:up)c=toupper(c);
                    if(up=="POS"||up=="NEG") trig_slope_=up;
                    else push_error("Execution error: bad slope",EXECUTION_ERROR);
                    return "";
                },
                [this](){ return trig_slope_; }},
            {{"TRIGger","DELay"},
                [this](const std::string& arg){
                    try{ trig_delay_=std::stod(arg);}
                    catch(...){ push_error("Execution error: bad delay",EXECUTION_ERROR);}
                    return "";
                },
                [this](){ return std::to_string(trig_delay_);} },

            // Control
            {{"INITiate"},
                [this](const std::string&){ measuring_=true; sesr_|=OPERATION_COMPLETE; return ""; }},
            {{"ABORt"},
                [this](const std::string&){ measuring_=false; return ""; }}
        };
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

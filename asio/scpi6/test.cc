#define BOOST_TEST_MODULE ScpiStubTests
#include <boost/test/included/unit_test.hpp>

#include "scpi_server.hpp"

// Import the stub implementation
class MyScpiSessionForTest : public ScpiSessionBase {
public:
    explicit MyScpiSessionForTest() : ScpiSessionBase(tcp::socket(io)) {
        load_commands(make_command_table());
    }

    // No real networking
    boost::asio::io_context io;

    // Expose command table (same as MyScpiSession in my_scpi_server.cpp)
    std::vector<ScpiCommandDef> make_command_table() {
        double* voltage = &voltage_;
        std::string* trig_source = &trig_source_;
        std::string* trig_slope = &trig_slope_;
        double* trig_level = &trig_level_;
        double* trig_delay = &trig_delay_;
        bool* measuring = &measuring_;

        return {
            {{"*IDN"}, {}, [this]() { return "MyCompany,SCPI-Stub,1234,0.7"; }},
            {{"*RST"}, [voltage](const std::string&){ *voltage=1.0; return ""; }},
            {{"SOURce","VOLTage"},
                [voltage,this](const std::string& arg){
                    try{ *voltage=std::stod(arg);}
                    catch(...){ push_error("bad voltage",EXECUTION_ERROR);}
                    return "";
                },
                [voltage](){ return std::to_string(*voltage);}
            },
            {{"MEASure","VOLTage"}, {}, [voltage](){ return std::to_string(*voltage+0.001);} },
            {{"TRIGger","SOURce"},
                [trig_source,this](const std::string& arg){
                    std::string up=arg; for(auto&c:up)c=toupper(c);
                    if(up=="IMM"||up=="EXT"||up=="BUS") *trig_source=up;
                    else push_error("bad trigger source",EXECUTION_ERROR);
                    return "";
                },
                [trig_source](){ return *trig_source; } }
        };
    }

private:
    double voltage_ = 1.0;
    std::string trig_source_ = "IMM";
    std::string trig_slope_  = "POS";
    double trig_level_ = 0.0;
    double trig_delay_ = 0.0;
    bool measuring_ = false;
};

// ---- Tests ----
BOOST_AUTO_TEST_CASE(idn_query) {
    MyScpiSessionForTest sess;
    std::string resp = sess.execute_command("*IDN?");
    BOOST_CHECK_EQUAL(resp, "MyCompany,SCPI-Stub,1234,0.7");
}

BOOST_AUTO_TEST_CASE(voltage_set_and_query) {
    MyScpiSessionForTest sess;
    sess.execute_command(":SOUR:VOLT 2.5");
    std::string resp = sess.execute_command(":SOUR:VOLT?");
    BOOST_CHECK_EQUAL(resp, "2.500000");
}§

BOOST_AUTO_TEST_CASE(measure_voltage) {
    MyScpiSessionForTest sess;
    sess.execute_command(":SOUR:VOLT 1.0");
    std::string resp = sess.execute_command(":MEAS:VOLT?");
    BOOST_CHECK_EQUAL(resp, "1.001000");
}

BOOST_AUTO_TEST_CASE(trigger_source_change) {
    MyScpiSessionForTest sess;
    sess.execute_command(":TRIG:SOUR EXT");
    std::string resp = sess.execute_command(":TRIG:SOUR?");
    BOOST_CHECK_EQUAL(resp, "EXT");
}

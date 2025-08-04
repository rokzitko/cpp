#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main() {
    try {
        net::io_context ioc;

        // Resolver and socket
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        auto const host = "echo.websocket.events";
        auto const port = "80";

        // Resolve domain
        auto const results = resolver.resolve(host, port);

        // Connect to the server
        net::connect(ws.next_layer(), results.begin(), results.end());

        // Perform WebSocket handshake
        ws.handshake(host, "/");

        // Send a message
        std::string msg = "Hello from C++ WebSocket client";
        ws.write(net::buffer(msg));

        // Receive a message
        beast::flat_buffer buffer;
        ws.read(buffer);
        std::cout << "Received: " << beast::make_printable(buffer.data()) << std::endl;

        // Close the connection
        ws.close(websocket::close_code::normal);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


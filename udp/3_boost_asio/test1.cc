
#include <iostream>
#include <boost/asio.hpp>

constexpr int PORT = 8080;
constexpr size_t BUFFER_SIZE = 1024;

int main() {
    try {
        // Create a context for I/O operations
        boost::asio::io_context io_context;

        // Create an endpoint for UDP on the given port
        boost::asio::ip::udp::endpoint server_endpoint(boost::asio::ip::udp::v4(), PORT);

        // Create a socket for the server
        boost::asio::ip::udp::socket socket(io_context, server_endpoint);

        std::array<char, BUFFER_SIZE> buffer;
        boost::asio::ip::udp::endpoint client_endpoint;

        std::cout << "Waiting for UDP packets on port " << PORT << "...\n";

        // Receive data from the client
        size_t len = socket.receive_from(boost::asio::buffer(buffer), client_endpoint);

        // Null-terminate the data for safe printing
        buffer[len] = '\0';

        std::cout << "Received packet: " << buffer.data() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}




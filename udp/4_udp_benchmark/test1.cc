#include <iostream>
#include <cstring>  // For memset
#include <cstdlib>  // For exit
#include <unistd.h> // For close
#include <arpa/inet.h>
#include <array>
#include <stdexcept>
#include <chrono>

constexpr int PORT = 60133;
constexpr size_t BUFFER_SIZE = 65536;

class UdpServer {
public:
	 UdpServer() {
		 // Create socket
		 sockfd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
		 if (sockfd_ < 0) {
			 throw std::runtime_error("Socket creation failed");
		 }
		 
		 // Set server address info
		 std::memset(&servaddr_, 0, sizeof(servaddr_));
		 servaddr_.sin_family = AF_INET;
		 servaddr_.sin_addr.s_addr = INADDR_ANY;
		 servaddr_.sin_port = htons(PORT);
		 
		 // Bind socket to the port
		 if (::bind(sockfd_, reinterpret_cast<struct sockaddr*>(&servaddr_), sizeof(servaddr_)) < 0) {
			 ::close(sockfd_);
			 throw std::runtime_error("Bind failed");
		 }
	 }
	 
	 ~UdpServer() {
		 // Close the socket upon destruction (RAII principle)
		 if (sockfd_ >= 0) {
			 ::close(sockfd_);
		 }
	 }
	 
	 void listen() {
		 std::cout << "Waiting for UDP packets on port " << PORT << "...\n";
		 
		 sockaddr_in cliaddr;
		 std::memset(&cliaddr, 0, sizeof(cliaddr));
		 
		 socklen_t len = sizeof(cliaddr);
		 std::array<char, BUFFER_SIZE> buffer;
		 
		 const auto begin = std::chrono::steady_clock::now();
		 size_t bytes {};
		 int seccnt = 0;
		 
		 for (;;) {
			 // Receive data from client
			 ssize_t n = ::recvfrom(sockfd_, buffer.data(), buffer.size(), 0, reinterpret_cast<struct sockaddr*>(&cliaddr), &len);
			 if (n < 0) {
				 throw std::runtime_error("Receive failed");
			 }
//			 std::cout << "Received packet: len=" << n << std::endl;
			 bytes += n;
			 const auto now = std::chrono::steady_clock::now();
			 const double microsecs = std::chrono::duration_cast<std::chrono::microseconds>(now - begin).count();
			 if (microsecs / 1000000 > seccnt) {
	       seccnt++;
				 const double transfer_rate = 8*bytes/microsecs; // result in units of Mbits/s
				 std::cout << "rate=" << transfer_rate << " Mbit/s" << std::endl;
       }       
		 }
	 }
		 
 private:
	 int sockfd_{-1};            // Socket file descriptor
	 sockaddr_in servaddr_;       // Server address info
};

int main() {
	try {
		UdpServer server;
			 server.listen();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

const int max_length = 1024;

void session(tcp::socket sock)
{
  try {
	  char data[max_length];
    for (int i = 0 ; i < 255; i++) 
      data[i] = 'x';
    data[0] = 'R';
    data[1] = 'o';
    data[2] = 'k';
    data[255] = 'Z';
    size_t length = 256;
	  boost::asio::write(sock, boost::asio::buffer(data, length));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_context& io_context, unsigned short port)
{
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;) {
    session( a.accept() );
  }
}

int main(int argc, char* argv[])
{
  try
    {
      if (argc != 2)
	{
	  std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
	  return 1;
	}

      boost::asio::io_context io_context;

      server(io_context, std::atoi(argv[1]));
    }
  catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }

  return 0;
}

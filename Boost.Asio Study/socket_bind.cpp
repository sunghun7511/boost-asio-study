#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int socket_bind_main(void) {

	unsigned short port = 80;

	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port);

	asio::io_service ios;

	asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
	boost::system::error_code ec;

	acceptor.bind(ep, ec);

	if (ec.value() != 0) {
		std::cout << "Failed to bind the acceptor socket.\n"
			<< "Error code : " << ec.value() << ", Message : " << ec.message();
		return ec.value();
	}

	return 0;
}
#include <boost/asio.hpp>
#include <iostream>

namespace udp_socket_bind {
	using namespace boost;

	int main(void) {
		unsigned short port = 80;
		asio::ip::udp::endpoint ep(asio::ip::address_v4::any(), port);
		asio::io_service ios;

		asio::ip::udp::socket sock(ios, ep.protocol());

		boost::system::error_code ec;

		sock.bind(ep, ec);

		if (ec.value() != 0) {
			std::cout << "Failed to bind the socket.\n"
				<< "Error code : " << ec.value() << ", Message : " << ec.message();
			return ec.value();
		}
		return 0;
	}
};
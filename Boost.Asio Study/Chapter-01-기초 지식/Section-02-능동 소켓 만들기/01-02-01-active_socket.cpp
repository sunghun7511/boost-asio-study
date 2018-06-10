#include <boost/asio.hpp>
#include <iostream>

namespace active_socket {
	using namespace boost;

	int main(void) {

		asio::io_service ios;

		asio::ip::tcp protocol = asio::ip::tcp::v4();

		asio::ip::tcp::socket sock(ios);

	boost:system::error_code ec;

		sock.open(protocol, ec);

		if (ec.value() != 0) {
			std::cout << "Failed to open the socket!\n"
				<< "Error code : " << ec.value() << ", Message : " << ec.message();
			return ec.value();
		}
		return 0;
	}
};
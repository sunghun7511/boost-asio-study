#include <boost/asio.hpp>
#include <iostream>

namespace passive_acceptor_socket {
	using namespace boost;

	int main(void) {
		asio::io_service ios;
		asio::ip::tcp protocol = asio::ip::tcp::v6();

		asio::ip::tcp::acceptor acceptor(ios);

		boost::system::error_code ec;

		acceptor.open(protocol, ec);

		if (ec.value() != 0) {
			std::cout << "Failed to open the acceptor socket!\n"
				<< "Error code : " << ec.value() << ", Message : " << ec.message();
			return ec.value();
		}
		return 0;
	}
}
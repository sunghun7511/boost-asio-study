#include <boost/asio.hpp>
#include <iostream>

namespace tcp_listen {
	using namespace boost;

	int main(void) {
		const int BACKLOG_SIZE = 30;

		unsigned short port = 3333;

		asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port);
		asio::io_service ios;

		try {
			asio::ip::tcp::acceptor acceptor(ios, ep.protocol());

			acceptor.bind(ep);

			acceptor.listen(BACKLOG_SIZE);

			asio::ip::tcp::socket sock(ios);

			acceptor.accept(sock);

			// sock is now connecting
		}
		catch (boost::system::system_error &e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();
			return e.code().value();
		}

		return 0;
	}
}
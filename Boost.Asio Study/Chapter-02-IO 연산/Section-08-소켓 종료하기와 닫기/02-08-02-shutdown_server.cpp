#include <boost/asio.hpp>
#include <iostream>

namespace shutdown_server {
	using namespace boost;

	void processRequest(asio::ip::tcp::socket& sock) {
		asio::streambuf request_buf;

		boost::system::error_code ec;

		asio::read(sock, request_buf, ec);

		if (ec != asio::error::eof)
			throw system::system_error(ec);

		const char response_buf[] = { 0x48, 0x69, 0x21 };

		asio::write(sock, asio::buffer(response_buf));

		sock.shutdown(asio::socket_base::shutdown_send);
	}

	int main(void) {
		unsigned short port = 3333;

		try {
			asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port);

			asio::io_service ios;

			asio::ip::tcp::acceptor acceptor(ios, ep);
			asio::ip::tcp::socket sock(ios);

			acceptor.accept(sock);

			processRequest(sock);
		}
		catch (boost::system::system_error& e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}

		return 0;
	}
}
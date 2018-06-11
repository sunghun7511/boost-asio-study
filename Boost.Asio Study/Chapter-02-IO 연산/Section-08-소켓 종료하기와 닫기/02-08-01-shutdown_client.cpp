#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace shutdown_client {
	using namespace boost;
	using std::string;

	void communicate(asio::ip::tcp::socket& sock) {
		const char request_buf[] = { 0x48, 0x65, 0x0, 0x6c, 0x6c, 0x6f };

		asio::write(sock, asio::buffer(request_buf));

		sock.shutdown(asio::socket_base::shutdown_send);

		asio::streambuf response_buf;

		boost::system::error_code ec;
		asio::read(sock, response_buf, ec);

		if (ec == asio::error::eof) {
			// all data was transfered.
		}
		else {
			throw boost::system::system_error(ec);
		}
	}

	int main(void) {
		string raw_ip = "127.0.0.1";
		unsigned short port = 3333;

		try {
			asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip), port);

			asio::io_service ios;

			asio::ip::tcp::socket sock(ios, ep.protocol());

			sock.connect(ep);

			communicate(sock);
		}
		catch (boost::system::system_error& e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}
		return 0;
	}
};
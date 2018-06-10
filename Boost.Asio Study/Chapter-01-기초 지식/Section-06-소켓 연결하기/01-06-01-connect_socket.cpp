#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace connect_socket {
	using namespace boost;
	using std::string;

	int main(void) {
		string raw_ip = "127.0.0.1";
		unsigned short port = 3333;

		try {
			asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip), port);
			asio::io_service ios;

			asio::ip::tcp::socket sock(ios, ep.protocol());

			sock.connect(ep);

			// connected!

		}
		catch (boost::system::system_error &e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();
			return e.code().value();
		}

		return 0;
	}
}
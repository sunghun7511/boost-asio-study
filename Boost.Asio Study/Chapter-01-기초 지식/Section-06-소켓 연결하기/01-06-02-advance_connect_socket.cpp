#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace advance_connect_socket {
	using namespace boost;
	using std::string;

	int main(void) {

		string host = "lapio.kr";
		string port = "80";

		asio::io_service ios;

		asio::ip::tcp::resolver::query resolver_query(host, port, asio::ip::tcp::resolver::query::numeric_service);
		asio::ip::tcp::resolver resolver(ios);

		try {
			asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);

			asio::ip::tcp::socket sock(ios);

			asio::connect(sock, it);

			// socket is now connect
		}
		catch (boost::system::system_error &e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}

		return 0;
	}
};
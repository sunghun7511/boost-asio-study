#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace dns_query {
	using std::string;
	using namespace boost;

	int main(void) {

		string host = "www.naver.com";
		string port = "80";

		asio::io_service ios;

		asio::ip::tcp::resolver::query resolver_query(host, port, asio::ip::tcp::resolver::query::numeric_service);
		asio::ip::tcp::resolver resolver(ios);

		boost::system::error_code ec;

		asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query, ec);

		if (ec.value() != 0) {
			std::cout << "Failed to resolve a DNS name.\n"
				<< "Error code : " << ec.value() << ", Message : " << ec.message();
			return ec.value();
		}

		asio::ip::tcp::resolver::iterator it_end;

		for (; it != it_end; ++it) {
			asio::ip::tcp::endpoint ep = it->endpoint();
			std::cout << ep.address().to_string() << "\n";
		}

		return 0;
	}
}
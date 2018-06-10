#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace asio_streambuf {
	using namespace boost;
	using std::string;

	int main(void) {

		asio::streambuf buf;

		std::ostream output(&buf);

		output << "Message1\nMessage2";

		std::istream input(&buf);

		std::string message1;

		std::getline(input, message1);

		return 0;
	}
}
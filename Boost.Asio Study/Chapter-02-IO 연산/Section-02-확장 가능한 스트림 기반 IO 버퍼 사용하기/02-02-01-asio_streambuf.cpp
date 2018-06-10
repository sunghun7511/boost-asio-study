#include <boost/asio.hpp>
#include <iostream>
#include <string>

using namespace boost;
using std::string;

int asio_streambuf_main(void) {

	asio::streambuf buf;

	std::ostream output(&buf);

	output << "Message1\nMessage2";

	std::istream input(&buf);

	std::string message1;

	std::getline(input, message1);

	return 0;
}
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using namespace boost;
using std::string;

int fixed_size_input_buffer_main(void) {
	const string buf = "Hello";

	asio::const_buffers_1 output_buf = asio::buffer(buf);

	return 0;
}
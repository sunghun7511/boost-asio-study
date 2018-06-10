#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace fixed_size_output_buffer {
	using namespace boost;
	using std::string;

	int main(void) {
		const string buf = "Hello";

		asio::const_buffers_1 output_buf = asio::buffer(buf);

		return 0;
	}
}
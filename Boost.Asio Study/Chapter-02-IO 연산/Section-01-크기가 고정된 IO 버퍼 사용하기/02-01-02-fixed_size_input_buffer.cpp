#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>

namespace fixed_size_input_buffer {
	using namespace boost;
	using std::string;

	int main(void) {
		const size_t BUF_SIZE_BYTES = 20;

		std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);

		asio::mutable_buffers_1 input_buf = asio::buffer(static_cast<void *>(buf.get()), BUF_SIZE_BYTES);

		return 0;
	}
};
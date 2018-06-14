#include <boost/asio.hpp>
#include <vector>

namespace combination_const_buffer {
	using namespace boost;

	int main(void) {
		const char * part1 = "Hello ";
		const char * part2 = "my ";
		const char * part3 = "friend!";

		std::vector<asio::const_buffer> composite_buffer;

		composite_buffer.push_back(asio::const_buffer(part1, 6));
		composite_buffer.push_back(asio::const_buffer(part2, 3));
		composite_buffer.push_back(asio::const_buffer(part3, 7));

		return 0;
	}
}
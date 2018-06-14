#include <boost/asio.hpp>
#include <vector>

namespace combination_mutable_buffer {
	using namespace boost;

	int main(void) {
		char part1[6];
		char part2[3];
		char part3[7];

		std::vector<asio::mutable_buffer> composite_buffer;

		composite_buffer.push_back(asio::mutable_buffer(part1, sizeof(part1)));
		composite_buffer.push_back(asio::mutable_buffer(part2, sizeof(part2)));
		composite_buffer.push_back(asio::mutable_buffer(part3, sizeof(part3)));

		return 0;
	}
}
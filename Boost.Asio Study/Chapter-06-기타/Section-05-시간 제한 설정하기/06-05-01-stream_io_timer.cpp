#include <boost/asio.hpp>
#include <iostream>

namespace stream_io {
	using namespace boost;

	int main(void) {
		asio::ip::tcp::iostream stream("localhost", "3333");
		if (!stream) {
			std::cout << "Error occured!\n"
				<< "Error code : " << stream.error().value()
				<< ", Message : " << stream.error().message() << std::endl;

			return stream.error().value();
		}

		stream.expires_from_now(std::chrono::seconds(5));

		stream << "Request.";
		stream.flush();

		std::cout << "Response : " << stream.rdbuf();

		return 0;
	}
}
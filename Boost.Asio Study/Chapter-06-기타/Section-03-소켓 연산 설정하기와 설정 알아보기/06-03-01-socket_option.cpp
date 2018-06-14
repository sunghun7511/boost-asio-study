#include <boost/asio.hpp>
#include <iostream>

namespace socket_option {
	using namespace boost;

	int main(void) {
		try {
			asio::io_service ios;

			asio::ip::tcp::socket sock(ios, asio::ip::tcp::v4());

			asio::socket_base::receive_buffer_size cur_buf_size;

			sock.get_option(cur_buf_size);

			std::cout << "Current receive buffer size is  " << cur_buf_size.value() << " bytes." << std::endl;

			asio::socket_base::receive_buffer_size new_buf_size(cur_buf_size.value() * 2);

			sock.set_option(new_buf_size);

			std::cout << "New receive buffer size is " << new_buf_size.value() << "bytes." << std::endl;

		}
		catch (boost::system::system_error& e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what() << std::endl;

			return e.code().value();
		}

		return 0;
	}
}
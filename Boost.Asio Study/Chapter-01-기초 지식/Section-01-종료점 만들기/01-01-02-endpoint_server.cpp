#include <boost/asio.hpp>
#include <iostream>

namespace endpoint_server {
	using namespace boost;

	int main(void) {

		unsigned short port_num = 3333;

		asio::ip::address ip_address = asio::ip::address_v6::any();

		asio::ip::tcp::endpoint ep(ip_address, port_num);


		return 0;
	}
}
#include <iostream>
#include <string>

#include <boost/asio.hpp>

using std::cout;
using std::cerr;

using std::string;

using namespace boost;

int client_main (void) {

    string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;

    boost::system::error_code ec;

    asio::ip::address ip_address = asio::ip::address::from_string(raw_ip_address, ec);

    if (ec.value() != 0) {
        cout << "Fail to parse the IP address.\n"
            << "Error code : " << ec.value() << ", Message : " << ec.message() << "\n";
        return ec.value();
    }

    asio::ip::tcp::endpoint ep(ip_address, port_num);



    return 0;
}
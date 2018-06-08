#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main (void) {

    asio::io_service ios;

    asio::ip::udp protocol = asio::ip::udp::v6();

    try {
        asio::ip::udp::socket sock(ios, protocol);
    } catch (boost::system::system_error& e) {
        std::cout << "Error occured!\n"
                  << "Error code : " << e.code() << ", Message : " << e.what();
        return e.code();
    }
    return 0;
}
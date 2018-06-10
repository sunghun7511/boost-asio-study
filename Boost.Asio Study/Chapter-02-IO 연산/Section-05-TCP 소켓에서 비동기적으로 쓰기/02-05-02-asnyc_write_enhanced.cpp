#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>

using namespace boost;
using std::string;
using std::shared_ptr;
using std::size_t;

using tcp_socket = asio::ip::tcp::socket;

struct Session {
	shared_ptr<tcp_socket> sock;
	shared_ptr<string> buf;
};

void callback(const boost::system::error_code& ec, size_t bytes_transferred, shared_ptr<Session> s) {
	if (ec.value() != 0) {
		std::cout << "Error occured!\n"
			<< "Error code : " << ec.value() << ", Mesasge : " << ec.message();
		return;
	}

	// do someting
}

void writeToSocket(shared_ptr<asio::ip::tcp::socket> sock, shared_ptr<string> buf) {

	shared_ptr<Session> s(new Session);

	s->buf = buf;
	s->sock = sock;

	asio::async_write(sock, asio::buffer(*s->buf),
		std::bind(callback, std::placeholders::_1, std::placeholders::_2, s)
	);
}

int async_write_main(void) {

	string raw_ip = "127.0.0.1";
	unsigned short port = 3333;

	shared_ptr<string> buf(new string("Hello"));

	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip), port);

		asio::io_service ios;

		shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));

		sock->connect(ep);

		writeToSocket(sock, buf);
	}
	catch (boost::system::system_error &e) {
		std::cout << "Error occured!\n"
			<< "Error code " << e.code().value() << ", Message : " << e.what();

		return e.code().value();
	}

	return 0;
}
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using namespace boost;
using std::string;

string readFromSocket(asio::ip::tcp::socket& sock) {
	const unsigned char MESSAGE_SIZE = 7;
	char buf[MESSAGE_SIZE];
	std::size_t total_bytes_read = 0;

	while (total_bytes_read != MESSAGE_SIZE) {
		total_bytes_read += sock.read_some(asio::buffer(buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read));
	}

	return string(buf, total_bytes_read);
}

string readFromSocketEnhanced(asio::ip::tcp::socket& sock) {
	const unsigned char MESSAGE_SIZE = 7;
	char buf[MESSAGE_SIZE];
	
	asio::read(sock, asio::buffer(buf, MESSAGE_SIZE));

	return string(buf, MESSAGE_SIZE);
}

string readFromSocketDelim(asio::ip::tcp::socket& sock) {
	asio::streambuf buf;

	asio::read_until(sock, buf, "\n");

	string message;

	std::istream input_stream(&buf);
	std::getline(input_stream, message);

	return message;
}

int sync_write_main(void) {
	string raw_ip = "127.0.0.1";
	unsigned short port = 3333;

	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip), port);
		asio::io_service ios;

		asio::ip::tcp::socket sock(ios, ep.protocol());

		sock.connect(ep);

		readFromSocket(sock);

		// or
		// 
		// readFromSocketEnhanced(sock);
	}
	catch (boost::system::system_error &e) {
		std::cout << "Error occured!\n"
			<< "Error code : " << e.code().value() << ", Message : " << e.what();

		return e.code().value();
	}

	return 0;
}
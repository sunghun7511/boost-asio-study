#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace sync_write {
	using namespace boost;
	using std::string;

	void writeToSocket(asio::ip::tcp::socket& sock, const string& buf) {
		std::size_t total_bytes_written = 0;

		while (total_bytes_written != buf.length()) {
			total_bytes_written += sock.write_some(asio::buffer(buf.c_str() + total_bytes_written, buf.length() - total_bytes_written));
		}
	}

	void writeToSocketEnhanced(asio::ip::tcp::socket& sock, const string& buf) {
		asio::write(sock, asio::buffer(buf));
	}

	int main(void) {
		string raw_ip = "127.0.0.1";
		unsigned short port = 3333;

		try {
			asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip), port);
			asio::io_service ios;

			asio::ip::tcp::socket sock(ios, ep.protocol());

			sock.connect(ep);

			writeToSocket(sock, "Hello");

			// or
			// 
			// writeToSocketEnhanced(sock, "Hello");
		}
		catch (boost::system::system_error &e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}

		return 0;
	}
};
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>

namespace async_write {
	using namespace boost;
	using std::string;

	struct Session {
		std::shared_ptr<asio::ip::tcp::socket> sock;
		std::shared_ptr<string> buf;
		std::size_t total_bytes_written;
	};

	void callback(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<Session> s) {
		if (ec.value() != 0) {
			std::cout << "Error occured!\n"
				<< "Error code : " << ec.value() << ", Mesasge : " << ec.message();
			return;
		}

		s->total_bytes_written += bytes_transferred;

		if (s->total_bytes_written == s->buf->length()) {
			return;
		}

		s->sock->async_write_some(asio::buffer(
			s->buf->c_str() + s->total_bytes_written,
			s->buf->length() - s->total_bytes_written
		), std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
	}

	void writeToSocket(std::shared_ptr<asio::ip::tcp::socket> sock, std::shared_ptr<string> buf) {

		std::shared_ptr<Session> s(new Session);

		s->buf = buf;
		s->total_bytes_written = 0;
		s->sock = sock;

		s->sock->async_write_some(asio::buffer(*s->buf),
			std::bind(callback, std::placeholders::_1, std::placeholders::_2, s)
		);
	}

	int main(void) {

		string raw_ip = "127.0.0.1";
		unsigned short port = 3333;

		std::shared_ptr<string> buf(new string("Hello"));

		try {
			asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip), port);

			asio::io_service ios;

			std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));

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
}
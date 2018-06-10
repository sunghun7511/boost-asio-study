#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>

namespace async_read_enhanced {
	using namespace boost;
	using std::string;

	struct Session {
		std::shared_ptr<asio::ip::tcp::socket> sock;
		std::unique_ptr<char[]> buf;
		unsigned int buf_size;
	};

	void callback(const boost::system::error_code& ec, std::size_t bytes_transfered, std::shared_ptr<Session> s) {
		if (ec.value() != 0) {
			std::cout << "Error occured!\n"
				<< "Error code : " << ec.value() << ", Mesasge : " << ec.message();
			return;
		}

		// do something
	}

	void readFromSocket(std::shared_ptr<asio::ip::tcp::socket> sock) {
		std::shared_ptr<Session> s(new Session);

		const unsigned int MESSAGE_SIZE = 7;

		s->buf.reset(new char[MESSAGE_SIZE]);
		
		s->sock = sock;
		s->buf_size = MESSAGE_SIZE;

		asio::async_read(*sock, asio::buffer(s->buf.get(), s->buf_size),
			std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));

	}

	int main(void) {
		string raw_ip = "127.0.0.1";
		unsigned short port = 3333;
		try {
			asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip), port);

			asio::io_service ios;

			std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));

			sock->connect(ep);

			readFromSocket(sock);

			ios.run();
		}
		catch (boost::system::system_error &e) {
			std::cout << "Error occured!\n"
				<< "Error code " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}

		return 0;
	}
}
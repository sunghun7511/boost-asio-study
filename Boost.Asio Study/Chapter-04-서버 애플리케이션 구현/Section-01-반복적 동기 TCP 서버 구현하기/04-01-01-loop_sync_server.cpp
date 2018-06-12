#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

namespace loop_sync_server {
	using namespace boost;
	using std::string;

	class Service {
	public:
		Service() {}

		void HandleClient(asio::ip::tcp::socket& sock) {
			try {
				asio::streambuf request;
				asio::read_until(sock, request, '\n');

				int i = 0;
				while (i != 1000000)
					i++;

				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				string response = "Response\n";
				asio::write(sock, asio::buffer(response));
			}
			catch (boost::system::system_error& e) {
				std::cout << "Error occured!\n"
					<< "Error code : " << e.code().value() << ", Message : " << e.what();
			}
		}
	private:
	};

	class Acceptor {
	public:
		Acceptor(asio::io_service& ios, unsigned short port) :
			m_ios(ios),
			m_acceptor(m_ios, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port)) {
			m_acceptor.listen();
		}

		void Accept() {
			asio::ip::tcp::socket sock(m_ios);

			m_acceptor.accept(sock);

			Service svc;
			svc.HandleClient(sock);
		}
	private:
		asio::io_service& m_ios;
		asio::ip::tcp::acceptor m_acceptor;
	};

	class Server {
	public:
		Server() : m_stop(false) { }

		void Start(unsigned short port) {
			m_thread.reset(new std::thread([this, port]() {
				Run(port);
			}));
		}

		void Stop() {
			m_stop.store(true);
			m_thread->join();
		}

	private:
		void Run(unsigned short port) {
			Acceptor acc(m_ios, port);

			while (!m_stop.load()) {
				acc.Accept();
			}
		}

		std::unique_ptr<std::thread> m_thread;
		std::atomic<bool> m_stop;
		asio::io_service m_ios;
	};

	int main(void) {
		unsigned short port = 3333;

		try {
			Server srv;
			srv.Start(port);

			std::this_thread::sleep_for(std::chrono::seconds(60));

			srv.Stop();
		}
		catch (boost::system::system_error& e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}

		return 0;
	}
}
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

namespace ssl_loop_sync_server {
	using namespace boost;
	using std::string;

	class Service {
	public:
		Service() {}

		void HandleClient(asio::ssl::stream<asio::ip::tcp::socket>& ssl_stream) {
			try {
				ssl_stream.handshake(asio::ssl::stream_base::server);

				asio::streambuf request;
				asio::read_until(ssl_stream, request, '\n');

				int i = 0;
				while (i != 1000000)
					i++;

				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				string response = "Response\n";
				asio::write(ssl_stream, asio::buffer(response));
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
			m_acceptor(m_ios, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port)),
			m_ssl_context(asio::ssl::context::sslv23_server){
			m_ssl_context.set_options(
				boost::asio::ssl::context::default_workarounds
				| boost::asio::ssl::context::no_sslv2
				| boost::asio::ssl::context::single_dh_use
			);

			m_ssl_context.set_password_callback(
				[this](std::size_t max_length, asio::ssl::context::password_purpose purpose)->string {
				return get_password(max_length, purpose);
			});

			m_ssl_context.use_certificate_chain_file("server.crt");
			m_ssl_context.use_private_key_file("server.key", boost::asio::ssl::context::pem);
			m_ssl_context.use_tmp_dh_file("dhparams.pem");


			m_acceptor.listen();
		}

		void Accept() {
			asio::ssl::stream<asio::ip::tcp::socket> ssl_stream(m_ios, m_ssl_context);

			m_acceptor.accept(ssl_stream.lowest_layer());

			Service svc;
			svc.HandleClient(ssl_stream);
		}
	private:

		string get_password(std::size_t max_length, asio::ssl::context::password_purpose purpose) const {
			return "pass";
		}
		
		asio::io_service& m_ios;
		asio::ip::tcp::acceptor m_acceptor;
		
		asio::ssl::context m_ssl_context;
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
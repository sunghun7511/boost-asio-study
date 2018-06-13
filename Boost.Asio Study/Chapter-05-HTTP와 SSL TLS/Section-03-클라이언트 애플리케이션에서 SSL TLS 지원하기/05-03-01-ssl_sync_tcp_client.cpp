#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <string>

namespace ssl_sync_tcp_client {

	using namespace boost;
	using std::string;

	class SyncSSLClient {
	public:
		SyncSSLClient(const string& raw_ip, unsigned short port) :
			m_ep(asio::ip::address::from_string(raw_ip), port),
			m_ssl_context(asio::ssl::context::sslv3_client),
			m_ssl_stream(m_ios, m_ssl_context) {

			m_ssl_stream.set_verify_mode(asio::ssl::verify_peer);

			m_ssl_stream.set_verify_callback(
				[this](bool preverified, asio::ssl::verify_context& context)->bool {
				return on_peer_verify(preverified, context);
			});
		}

		void connect() {
			m_ssl_stream.lowest_layer().connect(m_ep);
			m_ssl_stream.handshake(asio::ssl::stream_base::client);
		}

		void close() {
			boost::system::error_code ec;

			m_ssl_stream.shutdown(ec);

			m_ssl_stream.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
			m_ssl_stream.lowest_layer().close(ec);
		}

		string emulateLongComputationOp(unsigned int duration_sec) {
			string request = "EMULATE_LONG_COMP_OP " + std::to_string(duration_sec) + "\n";

			sendRequest(request);

			return receiveResponse();
		}

	private:

		bool on_peer_verify(bool preverified, asio::ssl::verify_context& context) {
			return true;
		}

		void sendRequest(const string& request) {
			asio::write(m_ssl_stream, asio::buffer(request));
		}

		string receiveResponse() {
			asio::streambuf buf;

			asio::read_until(m_ssl_stream, buf, '\n');

			std::istream input(&buf);

			string response;
			std::getline(input, response);

			return response;
		}

	private:
		asio::io_service m_ios;
		asio::ip::tcp::endpoint m_ep;

		asio::ssl::context m_ssl_context;
		asio::ssl::stream<asio::ip::tcp::socket> m_ssl_stream;
	};

	int main(void) {

		const string raw_ip = "127.0.0.1";
		const unsigned short port = 3333;

		try {
			SyncSSLClient client(raw_ip, port);

			client.connect();

			std::cout << "Sending request to the server... " << std::endl;

			string response = client.emulateLongComputationOp(10);

			std::cout << "Response received : " << response << std::endl;

			client.close();
		}
		catch (boost::system::system_error& e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}
		return 0;
	}
}
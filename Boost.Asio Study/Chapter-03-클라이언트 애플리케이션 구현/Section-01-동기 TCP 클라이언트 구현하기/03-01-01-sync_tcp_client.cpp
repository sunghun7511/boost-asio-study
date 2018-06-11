#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace sync_tcp_client {

	using namespace boost;
	using std::string;

	class SyncTCPClient {
	public:
		SyncTCPClient(const string& raw_ip, unsigned short port) :m_ep(asio::ip::address::from_string(raw_ip), port), m_sock(m_ios) {
			m_sock.open(m_ep.protocol());
		}

		void connect() {
			m_sock.connect(m_ep);
		}

		void close() {
			m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			m_sock.close();
		}

		string emulateLongComputationOp(unsigned int duration_sec) {
			string request = "EMULATE_LONG_COMP_OP " + std::to_string(duration_sec) + "\n";

			sendRequest(request);

			return receiveResponse();
		}

	private:
		void sendRequest(const string& request) {
			asio::write(m_sock, asio::buffer(request));
		}

		string receiveResponse() {
			asio::streambuf buf;

			asio::read_until(m_sock, buf, '\n');

			std::istream input(&buf);

			string response;
			std::getline(input, response);

			return response;
		}

	private:
		asio::io_service m_ios;

		asio::ip::tcp::endpoint m_ep;
		asio::ip::tcp::socket m_sock;
	};

	int main(void) {

		const string raw_ip = "127.0.0.1";
		const unsigned short port = 3333;

		try {
			SyncTCPClient client(raw_ip, port);

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
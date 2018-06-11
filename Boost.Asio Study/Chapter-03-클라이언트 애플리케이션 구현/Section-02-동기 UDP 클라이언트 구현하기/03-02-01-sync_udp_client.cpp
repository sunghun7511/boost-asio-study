#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace sync_udp_client {
	using namespace boost;
	using std::string;

	class SyncUDPClient {
	public:
		SyncUDPClient() : m_sock(m_ios) {
			m_sock.open(asio::ip::udp::v4());
		}

		string emulateLongComputationOp(unsigned int duration_sec, const string& raw_ip, unsigned short port) {
			string request = "EMULATE_LONG_COMP_OP " + std::to_string(duration_sec) + "\n";

			asio::ip::udp::endpoint ep(asio::ip::address::from_string(raw_ip), port);

			sendRequest(ep, request);
			return receiveResponse(ep);
		}
	private:
		void sendRequest(const asio::ip::udp::endpoint& ep, const string& request) {
			m_sock.send_to(asio::buffer(request), ep);
		}

		string receiveResponse(asio::ip::udp::endpoint& ep) {
			char response[6];
			std::size_t bytes_received = m_sock.receive_from(asio::buffer(response), ep);

			m_sock.shutdown(asio::ip::udp::socket::shutdown_both);

			return string(response, bytes_received);
		}

	private:
		asio::io_service m_ios;

		asio::ip::udp::socket m_sock;
	};

	int main(void) {
		const string server1_raw_ip = "127.0.0.1";
		const unsigned short server1_port = 3333;

		const string server2_raw_ip = "192.168.1.101";
		const unsigned short server2_port = 3334;

		try {
			SyncUDPClient client;

			std::cout << "Sending request to server #1 ... " << std::endl;

			string response = client.emulateLongComputationOp(10, server1_raw_ip, server1_port);

			std::cout << "Response from the server #1 received : " << response << std::endl;

			std::cout << "Sending request to the server #2 ... " << std::endl;

			response = client.emulateLongComputationOp(10, server2_raw_ip, server2_port);

			std::cout << "Response from the server #2 received : " << response << std::endl;
		}
		catch (boost::system::system_error& e) {
			std::cout << "Error occured!\n"
				<< "Error code : " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}
		return 0;
	}
}
#include <boost/predef.h>
#ifdef BOOST_OS_WINDOWS
#define _WIN32_WINNT 0x0501

#if _WIN32_WINNT <= 0x0502

#define BOOST_ASIO_DISABLE_IOCP
#define BOOST_ASIO_ENABLE_CANCELIO

#endif
#endif

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <string>

namespace cancel_async {

	using namespace boost;
	using std::string;

	int main(void) {
		string raw_ip = "127.0.0.1";
		unsigned short port = 3333;

		try {
			asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip), port);

			asio::io_service ios;

			std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));

			sock->async_connect(ep, [sock](const boost::system::error_code& ec) {
				if (ec.value() != 0) {
					if (ec == asio::error::operation_aborted) {
						std::cout << "Operation cancelled!";
					}
					else {
						std::cout << "Error occured!\n"
							<< "Error code : "
							<< ec.value()
							<< ", Message : "
							<< ec.message();
					}

					return;
				}

				// do something
			});

			std::thread worker_thread([&ios]() {
				try {
					ios.run();
				}
				catch (boost::system::system_error& e) {
					std::cout << "Error occured!\n"
						<< "Error code : " << e.code().value() << ", Message : " << e.what();
				}
			});
			
			std::this_thread::sleep_for(std::chrono::seconds(2));

			sock->cancel();

			worker_thread.join();
		}
		catch (boost::system::system_error& e) {
			std::cout << "Error occured!\n"
				<< "Error code " << e.code().value() << ", Message : " << e.what();

			return e.code().value();
		}

		return 0;
	}
}
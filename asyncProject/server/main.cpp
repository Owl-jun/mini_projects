#include <iostream>
#include <asio.hpp>
#include <thread>
using asio::ip::tcp;

class Session {
private:

public:

};


class Server {
private:
	tcp::acceptor acceptor;
public:
	Server(asio::io_context& io_context, short port)
		: acceptor(io_context, tcp::endpoint(tcp::v4(), port))
	{
		std::cout << "[Server::Server()] : ���� �����" << std::endl;
		start_accept();
	}

private:
	void start_accept() {
		auto socket = std::make_shared<tcp::socket>(acceptor.get_executor());
		std::cout << "[Server::start_accept] : Ŭ���̾�Ʈ ���� ���� ����" << std::endl;
		acceptor.async_accept(*socket, [this, socket](std::error_code ec) {
			if (!ec) {
				std::cout << "[Server::async_accept] : Ŭ���̾�Ʈ ���� �Ϸ�" << std::endl;
			}
			else {
				std::cout << "[Server::async_accept] : ���� -> " << ec.message() << std::endl;
			}
			start_accept();
		});
	}
};

int main() {
	asio::io_context io_context;
	Server server(io_context, 9000);
	std::thread acceptorThread([&io_context] { io_context.run(); });
	
	std::cout << "[main] : ���ὺ���� ���Ϸ���" << std::endl;
	
	acceptorThread.join();
	std::cout << "[main] : ���ὺ���� �����, ����Ȳ��" << std::endl;

	return 0;
}
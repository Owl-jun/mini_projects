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
	std::vector<std::thread> threads;
	for (int i = 0; i < 5; ++i) {
		threads.emplace_back(std::thread([&io_context] { io_context.run(); }));
	}
	
	std::cout << "[main] : ������� ���Ϸ���" << std::endl;
	
	for (auto& thread : threads) {
		thread.join();
		std::cout << "[main] : ������ �����" << std::endl;
	}
	std::cout << "[main] : ��� ������ �����" << std::endl;
	return 0;
}
#include "pch.h"
#include "Session.h"

class Server {
private:
	asio::ip::tcp::acceptor acceptor;
	asio::io_context& io_context;

public:
	Server(asio::io_context& io_context, short port)
		: acceptor(io_context, asio::ip::tcp::endpoint(tcp::v4(), port))
		, io_context(io_context)
	{
		std::cout << "[Server::Server()] : ���� �����" << std::endl;
		start_accept();
	}

private:
	void start_accept()
	{
		auto socket = std::make_shared<tcp::socket>(io_context);
		std::cout << "[Server::start_accept] : Ŭ���̾�Ʈ ���� ���� ����" << std::endl;
		acceptor.async_accept(*socket, [this, socket](std::error_code ec) {
			if (!ec) {
				std::cout << "[Server::async_accept] : Ŭ���̾�Ʈ ���� �Ϸ�" << std::endl;
				std::make_shared<Session>(socket)->start();
			}
			else {
				std::cout << "[Server::async_accept] : ���� -> " << ec.message() << std::endl;
			}
			start_accept();
			});
	}
};
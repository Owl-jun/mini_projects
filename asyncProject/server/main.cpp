#include <iostream>
#include <asio.hpp>
#include <thread>
#include <memory>
using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
private:
	std::shared_ptr<tcp::socket> socket;
	std::string read_msg;
public:
	Session(std::shared_ptr<tcp::socket> _socket) 
		: socket(_socket)
	{ 
	}

	void start()
	{
		std::cout << "[Session::start()] : 세션 시작됨" << std::endl;
		do_read();
	}

private:
	void do_read()
	{
		auto self(shared_from_this());
		asio::async_read_until(*socket, asio::dynamic_buffer(read_msg), "\n", [this, self](std::error_code ec, std::size_t length) {
			if (!ec) 
			{
				std::string msg = read_msg.substr(0, length - 1);
				read_msg.erase(0, length);
				std::cout << "[Session::do_read] : 클라이언트 -> " << msg << std::endl;

				do_read();
			}
			else
			{
				std::cout << "[Session::do_read] : 에러 발생 -> " << ec.message() << std::endl;
			}
			});
	}

	/*void do_write(const std::string& msg)
	{
		auto self(shared_from_this());
		asio::async_write(*socket, asio::buffer(msg),
			[this, self](std::error_code ec, std::size_t)
			{
				if (!ec)
				{
					do_write();
				}
				else
				{
					std::cout << "[Session::do_write()] 에러 발생 -> " << ec.message() << std::endl;
				}
			});
	}*/
};


class Server {
private:
	tcp::acceptor acceptor;
	asio::io_context& io_context;
public:
	Server(asio::io_context& io_context, short port)
		: acceptor(io_context, tcp::endpoint(tcp::v4(), port))
		, io_context(io_context)
	{
		std::cout << "[Server::Server()] : 연결 대기중" << std::endl;
		start_accept();
	}

private:
	void start_accept() {
		auto socket = std::make_shared<tcp::socket>(io_context);
		std::cout << "[Server::start_accept] : 클라이언트 연결 수신 시작" << std::endl;
		acceptor.async_accept(*socket, [this, socket](std::error_code ec) {
			if (!ec) {
				std::cout << "[Server::async_accept] : 클라이언트 연결 완료" << std::endl;
				std::make_shared<Session>(socket)->start();
			}
			else {
				std::cout << "[Server::async_accept] : 에러 -> " << ec.message() << std::endl;
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
	

	std::cout << "[main] : 스레드들 일하러감" << std::endl;
	
	for (auto& thread : threads) {
		thread.join();
		std::cout << "[main] : 스레드 퇴근함" << std::endl;
	}
	std::cout << "[main] : 모든 스레드 퇴근함" << std::endl;
	return 0;
}
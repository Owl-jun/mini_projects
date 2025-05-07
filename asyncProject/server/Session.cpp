#include "pch.h"
#include "Session.h"


Session::Session(std::shared_ptr<tcp::socket> _socket)
	: socket(_socket)
{
}

void Session::start()
{
	std::cout << "[Session::start()] : 세션 시작됨" << std::endl;
	do_read();
}

void Session::do_read()
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

void Session::do_write(const std::string& msg)
{
	//auto self(shared_from_this());
	//asio::async_write(*socket, asio::buffer(msg),
	//	[this, self](std::error_code ec, std::size_t)
	//	{
	//		if (!ec)
	//		{
	//			do_write();
	//		}
	//		else
	//		{
	//			std::cout << "[Session::do_write()] 에러 발생 -> " << ec.message() << std::endl;
	//		}
	//	});
}

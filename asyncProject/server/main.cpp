#include "pch.h"
#include "Server.hpp"

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
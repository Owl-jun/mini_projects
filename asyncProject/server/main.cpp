#include "pch.h"
#include "Server.hpp"

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
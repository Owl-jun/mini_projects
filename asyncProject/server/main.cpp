#include "pch.h"
#include "Server.hpp"
#include "QueueManager.h"
#include "SessionManager.hpp"

int main() {

	// ��Ʈ��ũ ���� �񵿱� �۾� ���
	asio::io_context io_context;
	Server server(io_context, 9000);
	
	// ���� ó�� ���� �Ŵ���
	QueueManager& QM = QueueManager::GetInstance();
	SessionManager& SM = SessionManager::GetInstance();

	// ��Ʈ��ũ ��� ��� ������ 1~2��
	std::vector<std::thread> NetThreads;
	for (int i = 0; i < 2; ++i) {
		NetThreads.emplace_back(std::thread([&io_context] { io_context.run(); }));
	}
	std::cout << "[main] : ��Ʈ��ũ ������ ���Ϸ���" << std::endl;

	// ���� ó�� ��� ������ 4��
	std::vector<std::thread> WorkerThreads;
	for (int i = 0; i < 4; ++i) {
		WorkerThreads.emplace_back(std::thread([&QM] { QM.run(); }));
	}
	std::cout << "[main] : ��Ŀ ������� ���Ϸ���" << std::endl;
	


	// ���Ϸ� ������ �θ���� �;� ��� �� �ִ� main ������༮ ..
	for (auto& NetThread : NetThreads) {
		NetThread.join();
	}
	std::cout << "[main] : ��Ʈ��ũ ������ �����" << std::endl;

	for (auto& workerthread : WorkerThreads) {
		workerthread.join();
		std::cout << "[main] : ��Ŀ ������ �����" << std::endl;
	}
	std::cout << "[main] : ��� ������ �����" << std::endl;

	return 0;
}
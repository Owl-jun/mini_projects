#pragma once
#include <queue>

class QueueManager
{
private:
	std::queue<Task> TaskQueue;
	std::mutex TaskMutex;
	std::condition_variable TaskCV;

	QueueManager() {}
	~QueueManager() = default;
	QueueManager(const QueueManager&) = delete;
	QueueManager& operator=(const QueueManager&) = delete;
public:
	static QueueManager& GetInstance() {
		static QueueManager instance;
		return instance;
	}

    void push(const Task& task);
	void run();

private:
    void process(Task& task); // ���� �۾� ���� ����
    

};


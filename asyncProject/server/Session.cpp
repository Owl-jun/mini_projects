#include "pch.h"
#include "Session.h"
#include "QueueManager.h"
#include "SessionManager.hpp"

Session::Session(std::shared_ptr<tcp::socket> _socket)
	: socket(_socket)
	, sending(false)
{
}

void Session::start()
{
	SessionManager::GetInstance().AddSession(shared_from_this());
	std::cout << "[Session::start()] : ���� ���۵�" << std::endl;
	do_read();
}


void Session::do_read()
{
	auto self(shared_from_this());
	asio::async_read_until(*socket, asio::dynamic_buffer(read_msg), "\n", [self](std::error_code ec, std::size_t length) {
		if (!ec)
		{
			std::string msg = self->read_msg.substr(0, length - 1);
			self->read_msg.erase(0, length);

			if (self->isValid(msg)) {
				QueueManager::GetInstance().push({ self,msg });
				//std::cout << "[Session::do_read] : Ŭ���̾�Ʈ -> " << msg << " Task Queue PUSH �Ϸ�." << std::endl;
			}
			else { std::cout << "[Session::do_read] : Ŭ���̾�Ʈ -> " << msg << " �߸��� ��Ŷ, ���ó�� �Ϸ�." << std::endl; }

			self->do_read();
		}
		else
		{
			std::cout << "[Session::do_read] : ���� �߻� -> " << ec.message() << std::endl;
			self->Close();
		}
		});
}

void Session::push_WriteQueue(std::shared_ptr<std::string> msg)
{
	std::unique_lock<std::mutex> lock(writeMutex);
	writeQueue.push(msg);
	//std::cout << "[Session::push_WriteQueue] : lock ȹ�� -> " << *msg << " writeQueue ��ϿϷ�." << std::endl;
	lock.unlock();

	if (!sending && !writeQueue.empty())
	{
		sending = true;
		do_write();
	}
}

void Session::do_write()
{
	std::lock_guard<std::mutex> lock(writeMutex);

	if (writeQueue.empty()) {
		sending = false;
		std::cout << "[Session::do_write] writeQueue �� ����� " << std::endl;
		return;
	}

	std::shared_ptr<std::string> msg = writeQueue.front();
	std::cout << "[Session::do_write] �۽� ���� -> " << *msg << std::endl;

	auto self(shared_from_this());
	asio::async_write(*socket, asio::buffer(*msg),
		[self](std::error_code ec, std::size_t)
		{
			std::lock_guard<std::mutex> lock(self->writeMutex);

			if (!ec)
			{
				self->writeQueue.pop();

				if (!self->writeQueue.empty())
				{
					self->do_write();
				}
				else 
				{
					self->sending = false;
				}
			}
			else
			{
				std::cout << "[Session::do_write()] ���� �߻� -> " << ec.message() << std::endl;
				self->Close();
			}
		});
}

bool Session::isValid(const std::string& packet)
{
	std::istringstream iss(packet);
	std::cout << "[Session::isValid] packet -> " << packet << std::endl;
	std::string id;
	iss >> id;
	std::cout << "[Session::isValid] id -> " << id << std::endl;
	std::vector<std::string>& validIDs = SessionManager::GetInstance().getValidIds();
	if (std::find(validIDs.begin(), validIDs.end(), id) != validIDs.end()) {
		std::cout << "[Session::isValid] ��ȿ�� �˻� -> true " << std::endl;
		return true;
	}
	else {
		std::cout << "[Session::isValid] ��ȿ�� �˻� -> false " << std::endl;
		return false;
	}
}

void Session::Close()
{
	std::cout << "[Session:Close] ���� �����" << std::endl;
	std::error_code ec;
	socket->close(ec);

	SessionManager::GetInstance().DelSession(socket);
}

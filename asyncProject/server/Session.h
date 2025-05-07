#pragma once
class Session : public std::enable_shared_from_this<Session> {
private:
	std::shared_ptr<tcp::socket> socket;
	std::string read_msg;
public:
	Session(std::shared_ptr<tcp::socket> _socket);
	void start();

private:
	void do_read();

	void do_write(const std::string& msg);

};
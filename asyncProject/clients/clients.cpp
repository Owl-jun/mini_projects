#include <iostream>
#include <asio.hpp>
#include <thread>
using asio::ip::tcp;

std::string read_msg;
void do_read(tcp::socket& _socket) {
    asio::async_read_until(_socket, asio::dynamic_buffer(read_msg), "\n",
        [&](std::error_code ec, std::size_t length) {
            if (!ec) {
                std::string rmsg = read_msg.substr(0, length - 1);
                read_msg.erase(0, length);
                std::cout << "수신 : " << rmsg << std::endl;
                do_read(_socket); // 다시 등록
            }
            else {
                std::cout << "읽기 에러 : " << ec.message() << std::endl;
            }
        });
}

int main()
{
    asio::io_context io_context;
    tcp::socket socket(io_context);
    tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 9000);
    std::string msg;

    socket.async_connect(endpoint, [&socket](std::error_code ec) {
        if (!ec) {
            std::cout << "서버접속 성공!" << std::endl;
        }
        else {
            std::cout << "에라에라 : " << ec.message() << std::endl;
        }
    });

    std::thread network([&io_context] {io_context.run(); });

    while (true) {
        std::getline(std::cin, msg);
        msg += '\n';

        socket.async_send(asio::buffer(msg),
            [&msg](std::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::cout << "[클라] " << bytes_transferred << " bytes 전송 완료" << std::endl;
                }
                else {
                    std::cout << "[클라] 전송 에러 -> " << ec.message() << std::endl;
                }
            });
        do_read(socket);
    }

    socket.close();
    network.join();
    std::cout << "프로그램을 종료합니데이 \n";

    return 0;
}

#include <iostream>
#include <asio.hpp>
#include <thread>
using asio::ip::tcp;

int main()
{
    asio::io_context io_context;
    tcp::socket socket(io_context);
    tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 9000);

    socket.async_connect(endpoint, [&socket](std::error_code ec) {
        if (!ec) {
            std::cout << "서버접속 성공!" << std::endl;
        }
        else {
            std::cout << "에라에라 : " << ec.message() << std::endl;
        }
    });

    std::thread network([&io_context] {io_context.run(); });

    std::string buffer;
    while (true) {
        std::cin >> buffer;
        std::cout << buffer << std::flush;
    }
    network.join();
    std::cout << "프로그램을 종료합니데이 \n";

    return 0;
}

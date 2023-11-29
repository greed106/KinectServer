#include <iostream>
#include <vector>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // 链接Winsock库


class SocketClient {
public:
    // 构造函数
    SocketClient(const std::string& serverAddr, int serverPort)
        : serverAddress(serverAddr), port(serverPort), socket(INVALID_SOCKET), connected(false) {
        // 初始化Winsock
        WSADATA wsa;
        std::cout << "Initializing Winsock..." << std::endl;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            std::cout << "Failed. Error Code: " << WSAGetLastError() << std::endl;
            throw std::runtime_error("Winsock initialization failed");
        }

        // 连接到服务器
        if (connectToServer()) {
            connected = true;
        }
    }

    // 析构函数
    ~SocketClient() {
        if (socket != INVALID_SOCKET) {
            closesocket(socket);
        }
        WSACleanup();
    }

    // 连接到服务器
    bool connectToServer() {
        // 创建socket
        socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (socket == INVALID_SOCKET) {
            std::cout << "Could not create socket: " << WSAGetLastError() << std::endl;
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr);

        // 连接到服务器
        if (connect(socket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
            std::cout << "Connect error" << std::endl;
            return false;
        }

        std::cout << "Connected to server" << std::endl;
        return true;
    }

    // 发送数据
    template <typename T>
    bool sendData(const std::vector<T>& data) {
        if (!connected) {
            std::cout << "Socket is not connected" << std::endl;
            return false;
        }

        int totalBytesSent = 0;
        while (totalBytesSent < data.size()) {
            int bytesSent = send(socket, &data[totalBytesSent], data.size() - totalBytesSent, 0);
            if (bytesSent == SOCKET_ERROR) {
                std::cout << "Send failed: " << WSAGetLastError() << std::endl;
                // 连接断开时尝试重新连接
                connected = connectToServer();
                return false;
            }
            totalBytesSent += bytesSent;
        }

        std::cout << "Data sent successfully" << std::endl;
        return true;
    }

private:
    std::string serverAddress;
    int port;
    SOCKET socket;
    bool connected;
};


int main() {
    try {
        // 创建客户端
        SocketClient client("localhost", 8888);

        // 连接到服务器
        if (!client.connectToServer()) {
            std::cerr << "Failed to connect to server" << std::endl;
            return 1;
        }

        // 发送数据
        std::vector<char> data = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd' };
        if (!client.sendData(data)) {
            std::cerr << "Failed to send data" << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>
#include <vector>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "kinect_image.h"

// 假设 kinect_image 和 color_point_t 已经定义，并且具备序列化功能

class SocketClient {
public:
    // 构造函数：初始化并连接到服务器
    SocketClient(const std::string& serverAddr, int serverPort)
        : serverAddress(serverAddr), port(serverPort), socket(INVALID_SOCKET), connected(false) {
        // 初始化Winsock
        WSADATA wsa;
        std::cout << "Initializing Winsock..." << std::endl;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            std::cout << "Failed. Error Code: " << WSAGetLastError() << std::endl;
            throw std::runtime_error("Winsock initialization failed");
        }
        // 尝试连接到服务器
        connected = connectToServer();
    }

    // 析构函数：关闭socket并清理Winsock
    ~SocketClient() {
        if (socket != INVALID_SOCKET) {
            closesocket(socket);
        }
        WSACleanup();
    }

    // 连接到服务器的函数
    bool connectToServer() {
        // 创建socket
        socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (socket == INVALID_SOCKET) {
            std::cout << "Could not create socket: " << WSAGetLastError() << std::endl;
            return false;
        }

        // 设置服务器地址
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

    // 发送 kinect_image 对象的函数
    bool sendImage(const kinect_image& image) {
        // 序列化 kinect_image 对象
        std::stringstream ss;
        boost::archive::text_oarchive oa(ss);
        oa << image;
        std::string serializedData = ss.str();
        // 发送序列化后的数据
        return sendData(serializedData);
    }

private:
    // 发送数据的函数
    bool sendData(const std::string& data) {
        if (!connected) {
            std::cout << "Socket is not connected" << std::endl;
            return false;
        }

        int totalBytesSent = 0;
        int dataSize = data.size();
        // 循环发送直到所有数据被发送
        while (totalBytesSent < dataSize) {
            int bytesSent = send(socket, data.c_str() + totalBytesSent, dataSize - totalBytesSent, 0);
            if (bytesSent == SOCKET_ERROR) {
                std::cout << "Send failed: " << WSAGetLastError() << std::endl;
                connected = connectToServer();
                return false;
            }
            totalBytesSent += bytesSent;
        }

        std::cout << "Data sent successfully" << std::endl;
        return true;
    }

    std::string serverAddress; // 服务器地址
    int port;                  // 服务器端口
    SOCKET socket;             // socket句柄
    bool connected;            // 连接状态标志
};

#include "KinectClient.h"
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>
#include <iostream>
#include <ws2tcpip.h>

SocketClient::SocketClient(const std::string& serverAddr, int serverPort)
    : serverAddress(serverAddr), port(serverPort), socket(INVALID_SOCKET), connected(false) {
    WSADATA wsa;
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "Failed. Error Code: " << WSAGetLastError() << std::endl;
        throw std::runtime_error("Winsock initialization failed");
    }
    connected = connectToServer();
}

SocketClient::~SocketClient() {
    if (socket != INVALID_SOCKET) {
        closesocket(socket);
    }
    WSACleanup();
}

bool SocketClient::connectToServer() {
    // 创建 socket
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket == INVALID_SOCKET) {
        std::cout << "Could not create socket: " << WSAGetLastError() << std::endl;
        return false;
    }

    // 设置服务器地址结构
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    // 将 IP 地址字符串转换为网络地址
    int status = inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr);
    if (status <= 0) {
        if (status == 0) {
            std::cout << "The server address is not in a valid format" << std::endl;
        }
        else {
            std::cout << "inet_pton error: " << WSAGetLastError() << std::endl;
        }
        closesocket(socket);
        return false;
    }

    // 尝试连接到服务器
    if (connect(socket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cout << "Connect error: " << WSAGetLastError() << std::endl;
        closesocket(socket);
        return false;
    }

    std::cout << "Connected to server" << std::endl;
    return true;
}


bool SocketClient::sendImage(const kinect_image& image) {

   

    std::stringstream ss;
    //boost::archive::text_oarchive oa(ss);
    
    
    boost::archive::binary_oarchive oa(ss);//尝试使用二进制传输

    auto start_time = std::chrono::high_resolution_clock::now();
    oa << image;

    
    std::string serializedData = ss.str();    
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;

    return sendData(serializedData);
}

bool SocketClient::sendData(const std::string& data) {
    if (!connected) {
        std::cout << "Socket is not connected" << std::endl;
        return false;
    }

    uint32_t dataSize = static_cast<uint32_t>(data.size());
    uint32_t networkDataSize = htonl(dataSize);
    int bytesSent = send(socket, reinterpret_cast<const char*>(&networkDataSize), sizeof(networkDataSize), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cout << "Send failed: " << WSAGetLastError() << std::endl;
        connected = connectToServer();
        return false;
    }

    int totalBytesSent = 0;
    while (totalBytesSent < dataSize) {
        bytesSent = send(socket, data.c_str() + totalBytesSent, dataSize - totalBytesSent, 0);
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

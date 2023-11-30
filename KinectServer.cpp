#pragma warning(disable : 4996)
#include "KinectServer.h"
#include <winsock2.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <ctime>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>

SocketServer::SocketServer(int port) : listenPort(port), listenSocket(INVALID_SOCKET) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw std::runtime_error("Winsock initialization failed");
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Socket creation failed");
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(listenPort);

    if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        closesocket(listenSocket);
        WSACleanup();
        throw std::runtime_error("Socket binding failed");
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(listenSocket);
        WSACleanup();
        throw std::runtime_error("Listen failed");
    }
}

SocketServer::~SocketServer() {
    closesocket(listenSocket);
    WSACleanup();
}

void SocketServer::run() {
    std::cout << "Waiting for client connections..." << std::endl;
    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Accept failed" << std::endl;
        }
        else {
            std::cout << "Client connected" << std::endl;
            std::thread clientThread(&SocketServer::handleClient, this, clientSocket);
            clientThread.detach();
        }
    }
}
std::string getPath() {
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 转换为时间戳（毫秒级）
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    // 将时间戳转换为日期时间格式
    auto time_point = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time_point);

    // 构建文件名
    std::ostringstream oss;
    oss << "static/";
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S"); // 将时间格式化为 "年-月-日-时-分-秒"
    oss << "-" << std::setw(3) << std::setfill('0') << milliseconds; // 添加毫秒部分
    oss << ".ply";

    // 获取文件路径
    std::string path = oss.str();
    return path;
}
void SocketServer::handleClient(SOCKET clientSocket) { 
    while (true) {
        try {
            kinect_image image = receiveImage(clientSocket);
            // TODO: 处理 kinect_image 对象
            // const std::string path = getPath();
            // image.writeToPly(path);
            std::cout<<"已成功接收到数据"<<std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            break;
        }
    }
    closesocket(clientSocket);
}

kinect_image SocketServer::receiveImage(SOCKET clientSocket) {
    uint32_t dataSize;
    int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&dataSize), sizeof(dataSize), 0);
    if (bytesReceived <= 0) {
        throw std::runtime_error("Failed to receive data size or connection closed");
    }

    dataSize = ntohl(dataSize);

    std::string receivedData;
    receivedData.reserve(dataSize);
    char buffer[512];
    uint32_t totalBytesReceived = 0;

    while (totalBytesReceived < dataSize) {
        bytesReceived = recv(clientSocket, buffer, min(sizeof(buffer), dataSize - totalBytesReceived), 0);
        if (bytesReceived > 0) {
            receivedData.append(buffer, bytesReceived);
            totalBytesReceived += bytesReceived;
        }
        else if (bytesReceived == 0) {
            throw std::runtime_error("Connection closed by client");
        }
        else {
            throw std::runtime_error("Receive failed");
        }
    }

    std::stringstream ss(receivedData);
    boost::archive::text_iarchive ia(ss);
    kinect_image image;
    ia >> image;

    return image;
}

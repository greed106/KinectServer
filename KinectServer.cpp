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
    // ��ȡ��ǰʱ���
    auto now = std::chrono::system_clock::now();

    // ת��Ϊʱ��������뼶��
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    // ��ʱ���ת��Ϊ����ʱ���ʽ
    auto time_point = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time_point);

    // �����ļ���
    std::ostringstream oss;
    oss << "static/";
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S"); // ��ʱ���ʽ��Ϊ "��-��-��-ʱ-��-��"
    oss << "-" << std::setw(3) << std::setfill('0') << milliseconds; // ��Ӻ��벿��
    oss << ".ply";

    // ��ȡ�ļ�·��
    std::string path = oss.str();
    return path;
}
void SocketServer::handleClient(SOCKET clientSocket) { 
    while (true) {
        try {
            kinect_image image = receiveImage(clientSocket);
            // TODO: ���� kinect_image ����
            // const std::string path = getPath();
            // image.writeToPly(path);
            std::cout<<"�ѳɹ����յ�����"<<std::endl;
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

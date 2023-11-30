#ifndef KINECT_SERVER_H
#define KINECT_SERVER_H

#include <winsock2.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include "KinectImage.h" // 确保这个文件存在并包含 kinect_image 和 color_point_t 的定义

class SocketServer {
public:
    SocketServer(int port);
    ~SocketServer();
    void run();

private:
    void handleClient(SOCKET clientSocket);
    kinect_image receiveImage(SOCKET clientSocket);

    int listenPort;
    SOCKET listenSocket;
};

#endif // KINECT_SERVER_H

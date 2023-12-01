#ifndef KINECT_SERVER_H
#define KINECT_SERVER_H

#include <winsock2.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <iomanip>
#include "KinectImage.h"


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

#endif

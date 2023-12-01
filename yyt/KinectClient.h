#ifndef KINECT_CLIENT_H
#define KINECT_CLIENT_H

#include <string>
#include <winsock2.h>
#include "KinectImage.h"
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>
#include <iostream>
#include <ws2tcpip.h>
#include <chrono>

class SocketClient {
public:
    SocketClient(const std::string& serverAddr, int serverPort);
    ~SocketClient();
    bool sendImage(const kinect_image& image);

private:
    bool connectToServer();
    bool sendData(const std::string& data);

    std::string serverAddress; // ��������ַ
    int port;                  // �������˿�
    SOCKET socket;             // socket���
    bool connected;            // ����״̬��־
};

#endif // KINECT_CLIENT_H

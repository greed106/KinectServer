#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "KinectServer.h"
#include "KinectClient.h"


const int port = 8888;
const std::string path = "D:\\vs_code(cpp)\\KinectServer\\static\\object.ply";

int main() {
	 // ������ʹ�÷���
	 SocketServer server = SocketServer(port);
	 std::cout << "����˳�ʼ�����" << std::endl;
	 server.run();

	// �ͻ���ʹ�÷���
	// const std::string ipAddr = "127.0.0.1";
	// SocketClient client(ipAddr, port);
	// client.sendImage(image);
	

	return 0;
}
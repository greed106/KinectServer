#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "KinectServer.h"


const int port = 8888;

int main() {

	SocketServer server = SocketServer(port);
	std::cout << "服务端初始化完毕" << std::endl;
	server.run();

	return 0;
}
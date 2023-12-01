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
	 // 服务器使用方法
	 SocketServer server = SocketServer(port);
	 std::cout << "服务端初始化完毕" << std::endl;
	 server.run();

	// 客户端使用方法
	// const std::string ipAddr = "127.0.0.1";
	// SocketClient client(ipAddr, port);
	// client.sendImage(image);
	

	return 0;
}
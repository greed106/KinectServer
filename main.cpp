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

	// 客户端使用方法（单线程发送）
	// const std::string ipAddr = "127.0.0.1";
	// SocketClient client(ipAddr, port);
	// client.sendImage(image);
	

	return 0;
}

//以下是客户端的main函数，因为发sao了还没封装好，sorry
//先用这个勉强用一下

//std::mutex mtx; // 用于同步的互斥锁
//std::chrono::duration<double, std::milli> sum;
//void sendImages(SocketClient& client, kinect_image& image, int clientId) {
//    for (int i = 0; i < 10; ++i) {
//        auto start = std::chrono::high_resolution_clock::now(); // 开始时间
//
//        client.sendImage(image);
//
//        auto end = std::chrono::high_resolution_clock::now();   // 结束时间
//        std::chrono::duration<double, std::milli> elapsed = end - start; // 计算耗时
//
//        mtx.lock(); // 加锁
//        std::cout << "客户端 " << clientId << " 发送图片 " << i + 1 << " 耗时: " << elapsed.count() << " 毫秒" << std::endl;
//        sum += elapsed;
//        mtx.unlock(); // 解锁
//    }
//
//    mtx.lock(); // 加锁
//    std::cout << "客户端 " << clientId << " 图片发送完毕" << std::endl;
//    mtx.unlock(); // 解锁
//}
//
//int main() {
//    kinect_image image;
//    image.loadImageFromFile(path, image);
//    std::cout << "点云读取完毕" << std::endl;
//
//
//
//    SocketClient client(ipAddr, port);
//    SocketClient client2(ipAddr, port);
//    SocketClient client3(ipAddr, port);
//    std::cout << "客户端创建完毕" << std::endl;
//
//    auto start = std::chrono::high_resolution_clock::now(); // 开始时间
//    // 创建并启动线程
//    std::thread thread1(sendImages, std::ref(client), std::ref(image), 1);
//    std::thread thread2(sendImages, std::ref(client2), std::ref(image), 2);
//    std::thread thread3(sendImages, std::ref(client3), std::ref(image), 3);
//
//    // 等待所有线程完成
//    thread1.join();
//    thread2.join();
//    thread3.join();
//
//    std::cout << "所有图片发送完毕" << std::endl;
//    std::cout << "单线程发送图片总耗时" << sum.count() << "毫秒" << std::endl;
//
//
//    auto end = std::chrono::high_resolution_clock::now();   // 结束时间
//    std::chrono::duration<double, std::milli> elapsed = end - start; // 计算耗时
//    std::cout << "多线程发送图片总耗时: " << elapsed.count() << " 毫秒" << std::endl;
//    return 0;
//}
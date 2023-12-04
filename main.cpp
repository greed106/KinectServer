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

	// �ͻ���ʹ�÷��������̷߳��ͣ�
	// const std::string ipAddr = "127.0.0.1";
	// SocketClient client(ipAddr, port);
	// client.sendImage(image);
	

	return 0;
}

//�����ǿͻ��˵�main��������Ϊ��sao�˻�û��װ�ã�sorry
//���������ǿ��һ��

//std::mutex mtx; // ����ͬ���Ļ�����
//std::chrono::duration<double, std::milli> sum;
//void sendImages(SocketClient& client, kinect_image& image, int clientId) {
//    for (int i = 0; i < 10; ++i) {
//        auto start = std::chrono::high_resolution_clock::now(); // ��ʼʱ��
//
//        client.sendImage(image);
//
//        auto end = std::chrono::high_resolution_clock::now();   // ����ʱ��
//        std::chrono::duration<double, std::milli> elapsed = end - start; // �����ʱ
//
//        mtx.lock(); // ����
//        std::cout << "�ͻ��� " << clientId << " ����ͼƬ " << i + 1 << " ��ʱ: " << elapsed.count() << " ����" << std::endl;
//        sum += elapsed;
//        mtx.unlock(); // ����
//    }
//
//    mtx.lock(); // ����
//    std::cout << "�ͻ��� " << clientId << " ͼƬ�������" << std::endl;
//    mtx.unlock(); // ����
//}
//
//int main() {
//    kinect_image image;
//    image.loadImageFromFile(path, image);
//    std::cout << "���ƶ�ȡ���" << std::endl;
//
//
//
//    SocketClient client(ipAddr, port);
//    SocketClient client2(ipAddr, port);
//    SocketClient client3(ipAddr, port);
//    std::cout << "�ͻ��˴������" << std::endl;
//
//    auto start = std::chrono::high_resolution_clock::now(); // ��ʼʱ��
//    // �����������߳�
//    std::thread thread1(sendImages, std::ref(client), std::ref(image), 1);
//    std::thread thread2(sendImages, std::ref(client2), std::ref(image), 2);
//    std::thread thread3(sendImages, std::ref(client3), std::ref(image), 3);
//
//    // �ȴ������߳����
//    thread1.join();
//    thread2.join();
//    thread3.join();
//
//    std::cout << "����ͼƬ�������" << std::endl;
//    std::cout << "���̷߳���ͼƬ�ܺ�ʱ" << sum.count() << "����" << std::endl;
//
//
//    auto end = std::chrono::high_resolution_clock::now();   // ����ʱ��
//    std::chrono::duration<double, std::milli> elapsed = end - start; // �����ʱ
//    std::cout << "���̷߳���ͼƬ�ܺ�ʱ: " << elapsed.count() << " ����" << std::endl;
//    return 0;
//}
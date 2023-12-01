#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "KinectClient.h"
#include "KinectImage.h"


const int port = 8888;
const std::string ipAddr = "127.0.0.1";
const std::string path = "D:\\CODE\\kinect_client\\static\\object.ply";


void load_kinect_image_from_file(const std::string& file_path, kinect_image& image) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << file_path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        color_point_t point;

        // 读取 xyz 和 rgb 数据
        for (int i = 0; i < 3; ++i) {
            float temp;
            iss >> temp;
            point.xyz[i] = static_cast<int16_t>(std::round(temp)); // 将浮点数四舍五入为整数
        }
        for (int i = 0; i < 3; ++i) {
            int color;
            iss >> color;
            point.rgb[i] = static_cast<uint8_t>(color);
        }

        // 将点添加到 kinect_image
        image.push(point);
    }

    file.close();
}

int main() {

	kinect_image image;
    load_kinect_image_from_file(path, image);
	std::cout << "点云读取完毕" << std::endl;
	SocketClient client(ipAddr, port);
	std::cout << "客户端创建完毕" << std::endl;

    //auto start_time = std::chrono::high_resolution_clock::now();

	client.sendImage(image);
    //client.sendImage(image);
    //client.sendImage(image);
    //client.sendImage(image);
    //client.sendImage(image);
    //client.sendImage(image);
    //client.sendImage(image);
    //auto end_time = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    //std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
	return 0;
}

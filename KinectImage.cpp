#include "KinectImage.h"

void kinect_image::push(color_point_t point) {
    points.push_back(point);
}

std::vector<color_point_t> kinect_image::get_points() {
    return points;
}

void kinect_image::writeToPly(const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // 写入 PLY 头部
    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << points.size() << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "property uchar red\n";
    file << "property uchar green\n";
    file << "property uchar blue\n";
    file << "end_header\n";

    // 写入点数据
    for (const auto& point : points) {
        // 将 int16_t 坐标转换为浮点数
        file << static_cast<float>(point.xyz[0]) << ' ';
        file << static_cast<float>(point.xyz[1]) << ' ';
        file << static_cast<float>(point.xyz[2]) << ' ';

        // 确保颜色值作为数值而非字符被写入
        file << static_cast<unsigned>(point.rgb[0]) << ' ';
        file << static_cast<unsigned>(point.rgb[1]) << ' ';
        file << static_cast<unsigned>(point.rgb[2]) << '\n';
    }

    file.close();
}

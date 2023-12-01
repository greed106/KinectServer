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

    // д�� PLY ͷ��
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

    // д�������
    for (const auto& point : points) {
        // �� int16_t ����ת��Ϊ������
        file << static_cast<float>(point.xyz[0]) << ' ';
        file << static_cast<float>(point.xyz[1]) << ' ';
        file << static_cast<float>(point.xyz[2]) << ' ';

        // ȷ����ɫֵ��Ϊ��ֵ�����ַ���д��
        file << static_cast<unsigned>(point.rgb[0]) << ' ';
        file << static_cast<unsigned>(point.rgb[1]) << ' ';
        file << static_cast<unsigned>(point.rgb[2]) << '\n';
    }

    file.close();
}

std::string kinect_image::serializeToString(kinect_image& image)
{
    // ʹ�� YAS ���л�
    yas::mem_ostream os;
    yas::binary_oarchive<yas::mem_ostream> oarchive(os);
    oarchive& image;

    // ��ȡ���л�������ݲ�ת��Ϊ�ַ���
    auto serialized_data = os.get_intrusive_buffer();
    std::string serialized_str(serialized_data.data, serialized_data.size);
    return serialized_str;
}

kinect_image kinect_image::deserializeFromString(const std::string& serialized_str)
{
    // ����������
    yas::mem_istream is(serialized_str.data(), serialized_str.size());

    // ���������л���
    yas::binary_iarchive<yas::mem_istream> iarchive(is);

    // �����л�
    kinect_image img;
    iarchive& img;

    return img;
}
void kinect_image::loadImageFromFile(const std::string& file_path, kinect_image& image) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�: " << file_path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        color_point_t point;

        // ��ȡ xyz �� rgb ����
        for (int i = 0; i < 3; ++i) {
            float temp;
            iss >> temp;
            point.xyz[i] = static_cast<int16_t>(std::round(temp)); // ����������������Ϊ����
        }
        for (int i = 0; i < 3; ++i) {
            int color;
            iss >> color;
            point.rgb[i] = static_cast<uint8_t>(color);
        }

        // ������ӵ� kinect_image
        image.push(point);
    }

    file.close();
}
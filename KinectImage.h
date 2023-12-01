#ifndef KINECT_IMAGE_H
#define KINECT_IMAGE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "yas/serialize.hpp"
#include "yas/std_types.hpp"

// ���� color_point_t �ṹ��
struct color_point_t {
    int16_t xyz[3]; // XYZ ����
    uint8_t rgb[3]; // RGB ��ɫ
};

// Ϊ color_point_t �ṩ���л�����
template<typename Ar>
void serialize(Ar& ar, color_point_t& p) {
    ar& YAS_OBJECT_NVP(
        "color_point",
        ("xyz", p.xyz),
        ("rgb", p.rgb)
    );
}

// ���� kinect_image ��
class kinect_image {
public:
    std::vector<color_point_t> points;

    // Ϊ kinect_image �ṩ���л�����
    template<typename Ar>
    void serialize(Ar& ar) {
        ar& YAS_OBJECT_NVP(
            "kinect_image",
            ("points", points)
        );
    }

    void push(color_point_t point);
    std::vector<color_point_t> get_points();
    void writeToPly(const std::string& filename);
    static std::string serializeToString(kinect_image& image);
    static kinect_image deserializeFromString(const std::string& str);
    void loadImageFromFile(const std::string& file_path, kinect_image& image);
};

#endif // KINECT_IMAGE_H

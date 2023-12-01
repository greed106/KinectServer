#ifndef KINECT_IMAGE_H
#define KINECT_IMAGE_H

#include <fstream>
#include <iomanip>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// 定义 color_point_t 结构体
struct color_point_t {
    int16_t xyz[3]; // XYZ 坐标
    uint8_t rgb[3]; // RGB 颜色


    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& xyz;
        ar& rgb;
    }
};

// 定义 kinect_image 类
class kinect_image {
public:
    std::vector<color_point_t> points;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& points;
    }

    void push(color_point_t point);
    std::vector<color_point_t> get_points();
    void writeToPly(const std::string& filename);
};

BOOST_CLASS_TRACKING(kinect_image, boost::serialization::track_never)

#endif // KINECT_IMAGE_H

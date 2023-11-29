#ifndef KINECT_IMAGE_H
#define KINECT_IMAGE_H

#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// 定义 color_point_t 结构体
struct color_point_t {
    int16_t xyz[3]; // XYZ 坐标
    uint8_t rgb[3]; // RGB 颜色

    // 序列化 color_point_t
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& xyz;
        ar& rgb;
    }
};

// 定义 kinect_image 类
class kinect_image {
private:
    std::vector<color_point_t> points; // 存储颜色点的向量

    // 序列化 kinect_image
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& points;
    }

public:
    // 向图像中添加一个点
    void push(color_point_t point);

    // 获取图像中的所有点
    std::vector<color_point_t> get_points();
};

// 注册 kinect_image 类以进行序列化
BOOST_CLASS_TRACKING(kinect_image, boost::serialization::track_never)

#endif // KINECT_IMAGE_H

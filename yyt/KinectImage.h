#ifndef KINECT_IMAGE_H
#define KINECT_IMAGE_H

#include <fstream>
#include <iomanip>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// ���� color_point_t �ṹ��
struct color_point_t {
    int16_t xyz[3]; // XYZ ����
    uint8_t rgb[3]; // RGB ��ɫ


    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& xyz;
        ar& rgb;
    }
};

// ���� kinect_image ��
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

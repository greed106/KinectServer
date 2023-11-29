#ifndef KINECT_IMAGE_H
#define KINECT_IMAGE_H

#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// ���� color_point_t �ṹ��
struct color_point_t {
    int16_t xyz[3]; // XYZ ����
    uint8_t rgb[3]; // RGB ��ɫ

    // ���л� color_point_t
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& xyz;
        ar& rgb;
    }
};

// ���� kinect_image ��
class kinect_image {
private:
    std::vector<color_point_t> points; // �洢��ɫ�������

    // ���л� kinect_image
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& points;
    }

public:
    // ��ͼ�������һ����
    void push(color_point_t point);

    // ��ȡͼ���е����е�
    std::vector<color_point_t> get_points();
};

// ע�� kinect_image ���Խ������л�
BOOST_CLASS_TRACKING(kinect_image, boost::serialization::track_never)

#endif // KINECT_IMAGE_H

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <vector>
#include <sstream>

struct color_point_t {
    int16_t xyz[3];
    uint8_t rgb[3];

    // Serialize color_point_t
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& xyz;
        ar& rgb;
    }
};

class kinect_image {
private:
    std::vector<color_point_t> points;

    // Serialize kinect_image
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& points;
    }

public:
    void push(color_point_t point) {
        points.push_back(point);
    }

    std::vector<color_point_t> get_points() {
        return points;
    }
};

// Register kinect_image for serialization
BOOST_CLASS_TRACKING(kinect_image, boost::serialization::track_never)
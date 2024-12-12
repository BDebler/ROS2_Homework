#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/header.hpp"
#include <chrono>
#include <cv_bridge/cv_bridge.h> // cv_bridge converts between ROS 2 image messages and OpenCV image representations.
#include <opencv2/opencv.hpp> // We include everything about OpenCV as we don't care much about compilation time at the moment.


class ColorSpotter : public rclcpp::Node
{
public:
    ColorSpotter() : Node("color_spotter")
    {
        // Subscribe to the input image topic
        image_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/camera1/image_raw", 10, std::bind(&ColorSpotter::image_callback, this, std::placeholders::_1));

        // Publisher for processed image
        _detected_img = this->create_publisher<sensor_msgs::msg::Image>( "/detected_image", 1);
    }

private:
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception& e)  {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            return;
        }

        // Copy the image locally
        cv::Mat working_img = cv_ptr->image.clone();
        cv::Mat output_img = cv_ptr->image.clone();

        // Convert the image to HSV color space
        cv::Mat imgHSV;
        cv::cvtColor(working_img, imgHSV, cv::COLOR_BGR2HSV);

        // Define the lower and upper bounds of the color red
        cv::Scalar lowerRed1(0, 120, 70);   // Lower bound for the first red range
        cv::Scalar upperRed1(10, 255, 255); // Upper bound for the first red range

        cv::Scalar lowerRed2(170, 120, 70);  // Lower bound for the second red range
        cv::Scalar upperRed2(180, 255, 255); // Upper bound for the second red range

        // Threshold the image to get only red colors
        cv::Mat mask1, mask2, mask;
        cv::inRange(imgHSV, lowerRed1, upperRed1, mask1);
        cv::inRange(imgHSV, lowerRed2, upperRed2, mask2);

        // Combine the two masks
        mask = mask1 | mask2;

        // Perform morphological operations to clean the mask
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

        // Find contours in the mask
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Prepare bounding box and color information
        std::ostringstream metadata;
        std::string detectedColor = "red";

        for (size_t i = 0; i < contours.size(); ++i) {
            cv::Rect boundingBox = cv::boundingRect(contours[i]);

            // Append bounding box data and color name to the metadata string
            metadata << boundingBox.x << "," << boundingBox.y << ","
                     << boundingBox.width << "," << boundingBox.height << ","
                     << detectedColor;

            if (i < contours.size() - 1) {
                metadata << ";";
            }

            // Optionally draw the rectangle and color name on the image
            cv::rectangle(output_img, boundingBox, cv::Scalar(153, 0, 0), 2);
            cv::putText(output_img, detectedColor, cv::Point(boundingBox.x, boundingBox.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(153, 0, 0), 2);
        }

        // Convert to message
        msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "rgb8", output_img).toImageMsg();

        // Embed bounding box data in the message header
        msg_->header.frame_id = metadata.str();

        // Send it out
        _detected_img->publish(*msg_.get());
    }

   
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_subscription_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr _detected_img;
    sensor_msgs::msg::Image::SharedPtr msg_;

};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ColorSpotter>());
    rclcpp::shutdown();
    return 0;
}

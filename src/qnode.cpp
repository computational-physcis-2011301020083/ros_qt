/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/
#include <ros/ros.h>
#include <ros/network.h>
#include <std_msgs/Float64MultiArray.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <iomanip>
#include "../include/ros_qt_demo/qnode.hpp"  // 只包含自身的头文件

namespace class1_ros_qt_demo {

QNode::QNode(int argc, char** argv)
    : init_argc(argc)
    , init_argv(argv)
    , image_transport(nullptr)
{}

QNode::~QNode()
{
    if (ros::isStarted()) {
        ros::shutdown();
        ros::waitForShutdown();
    }
    wait();

    // 清理资源
    if (image_transport) {
        delete image_transport;
        image_transport = nullptr;
    }

}

bool QNode::init()
{
    ros::init(init_argc, init_argv, "class1_ros_qt_demo");
    if (!ros::master::check()) {
        return false;
    }
    ros::start();
    ros::NodeHandle n;
    topic_subscriber = n.subscribe("model_sensor", 10, &QNode::topicCallback, this);

    // 初始化图像传输
    image_transport = new image_transport::ImageTransport(n);
    image_subscriber = image_transport->subscribe("camera/image_raw", 10, &QNode::imageCallback, this);

    start();
    return true;
}

void QNode::run()
{
    ros::Rate loop_rate(30);  // 30Hz循环
    while (ros::ok()) {
        ros::spinOnce();
        loop_rate.sleep();
    }
    ROS_INFO("Ros shutdown, proceeding to close the gui.");
    Q_EMIT rosShutdown();
}

void QNode::topicCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
    std::stringstream dataStream;
    dataStream << std::fixed << std::setprecision(2);
    dataStream << "Sensor State(degree): ";
    dataStream << "Excavator Swing Angle: " << msg->data[0]*180/M_PI<< ", ";
    dataStream << "Boom Angle: " << msg->data[1]*180/M_PI<< ", ";
    dataStream << "Arm Angle: " << msg->data[2]*180/M_PI<< ", ";
    dataStream << "Bucket Angle: " << msg->data[3]*180/M_PI<< ".";
    Q_EMIT topicDataReceived(dataStream.str());
}

// 实现图像回调函数：将ROS图像转换为QImage并发送信号
void QNode::imageCallback(const sensor_msgs::Image::ConstPtr& msg)
{
    if (!msg) return;  // 检查消息指针是否有效

    try {
        // 尝试转换为RGB8格式
        cv_bridge::CvImageConstPtr cv_ptr;
        if (msg->encoding == "bgr8") {
            cv_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8);
        } else {
            // 转换为RGB8格式
            cv_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::RGB8);
        }

        const cv::Mat& cv_image = cv_ptr->image;
        if (cv_image.empty()) {
            ROS_WARN("Received empty image!");
            return;
        }

        QImage q_image;
        if (msg->encoding == "bgr8") {
            // BGR到RGB的转换
            q_image = QImage(cv_image.data,
                           cv_image.cols,
                           cv_image.rows,
                           cv_image.step,
                           QImage::Format_RGB888).rgbSwapped();
        } else {
            // 直接使用RGB格式
            q_image = QImage(cv_image.data,
                           cv_image.cols,
                           cv_image.rows,
                           cv_image.step,
                           QImage::Format_RGB888);
        }

        // 确保图像数据深拷贝，避免内存问题
        q_image = q_image.copy();

        Q_EMIT imageReceived(q_image);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    } catch (...) {
        ROS_ERROR("Unknown error while processing image!");
    }
}

}  // namespace class1_ros_qt_demo

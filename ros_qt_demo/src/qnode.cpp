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
{}

QNode::~QNode()
{
    if (ros::isStarted()) {
        ros::shutdown();
        ros::waitForShutdown();
    }
    wait();

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

    // 初始化图像传输并订阅摄像头话题
    image_transport::ImageTransport it(n);
    image_subscriber = it.subscribe("camera/image_raw", 100, &QNode::imageCallback, this);

    start();
    return true;
}

void QNode::run()
{
    ros::spin();
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
    try {
        // 将ROS图像消息转换为OpenCV格式，使用RGB8格式
        cv_bridge::CvImageConstPtr cv_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::RGB8);
        cv::Mat cv_image = cv_ptr->image;

        // 转换为QImage，使用Qt 5支持的Format_RGB888格式
        QImage q_image(cv_image.data,
                      cv_image.cols,
                      cv_image.rows,
                      cv_image.step,
                      QImage::Format_RGB888);  // 修正为Qt 5支持的格式

        // 发送信号到UI线程
        Q_EMIT imageReceived(q_image);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }
}

}  // namespace class1_ros_qt_demo

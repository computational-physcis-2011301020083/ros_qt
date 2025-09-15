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
#include <sstream>
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
    topic_subscriber = n.subscribe("model_sensor", 1000, &QNode::topicCallback, this);
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

}  // namespace class1_ros_qt_demo

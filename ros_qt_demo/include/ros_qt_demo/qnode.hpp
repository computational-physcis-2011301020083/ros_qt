/**
 * @file /include/class1_ros_qt_demo/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
#ifndef class1_ros_qt_demo_QNODE_HPP_
#define class1_ros_qt_demo_QNODE_HPP_

#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>  // 用于图像传输
#include <cv_bridge/cv_bridge.h>             // 用于ROS图像与OpenCV转换
#endif
#include <string>
#include <QThread>
#include <QImage>  // 用于Qt图像显示

namespace class1_ros_qt_demo {

class QNode : public QThread {
    Q_OBJECT

public:
    QNode(int argc, char** argv);
    virtual ~QNode();
    bool init();
    void run();

Q_SIGNALS:
    void rosShutdown();
    void topicDataReceived(const std::string& data);
    void imageReceived(const QImage& image);  // 新增图像信号

private:
    int init_argc;
    char** init_argv;
    ros::Subscriber topic_subscriber;
    void topicCallback(const std_msgs::Float64MultiArray::ConstPtr& msg);
    image_transport::Subscriber image_subscriber;  // 图像订阅器
    void imageCallback(const sensor_msgs::Image::ConstPtr& msg);  // 图像回调函数
};

}  // namespace class1_ros_qt_demo

#endif /* class1_ros_qt_demo_QNODE_HPP_ */

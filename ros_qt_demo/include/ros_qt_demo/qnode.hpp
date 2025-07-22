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
#endif
#include <string>
#include <QThread>

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

private:
    int init_argc;
    char** init_argv;
    ros::Subscriber topic_subscriber;
    void topicCallback(const std_msgs::Float64MultiArray::ConstPtr& msg);
};

}  // namespace class1_ros_qt_demo

#endif /* class1_ros_qt_demo_QNODE_HPP_ */

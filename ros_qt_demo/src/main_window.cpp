/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
#include <QCloseEvent>
#include <QToolBar>
#include <QStatusBar>
#include "../include/ros_qt_demo/main_window.hpp"

namespace class1_ros_qt_demo {

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
    : QMainWindow(parent)
    , qnode(argc, argv)
{
    // 注册std::string类型，使其可以在信号槽中传递
    qRegisterMetaType<std::string>("std::string");

    // 创建工具栏
    toolBar = addToolBar("ToolBar");

    // 创建状态栏
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
    QObject::connect(&qnode, SIGNAL(topicDataReceived(const std::string&)), this, SLOT(updateStatusBar(const std::string&)));

    if (qnode.init()) {
        toolBar->setEnabled(true);
    }
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

void MainWindow::updateStatusBar(const std::string& data)
{
    statusBar->showMessage(QString::fromStdString(data));
}

}  // namespace class1_ros_qt_demo


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
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMetaType>
#include "../include/ros_qt_demo/main_window.hpp"

namespace class1_ros_qt_demo {

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
    : QMainWindow(parent)
    , qnode(argc, argv)
{
    // 注册std::string类型，使其可以在信号槽中传递
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<QImage>("QImage");

    // 创建工具栏
    toolBar = addToolBar("ToolBar");

    // 创建状态栏
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    // 创建中心部件和布局
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);
    // 创建图像显示标签并设置为左上角四分之一区域
    image_label = new QLabel(this);
    image_label->setText("Waiting for image...");
    image_label->setAlignment(Qt::AlignCenter);
    image_label->setStyleSheet("border: 1px solid black;");  // 添加边框
    mainLayout->addWidget(image_label);

    // 连接信号槽
    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
    QObject::connect(&qnode, SIGNAL(topicDataReceived(const std::string&)),this, SLOT(updateStatusBar(const std::string&)));
    QObject::connect(&qnode, SIGNAL(imageReceived(const QImage&)),this, SLOT(updateImage(const QImage&)));
    //setCentralWidget(image_label);  // 如果没有其他中心部件

    if (qnode.init()) {
        toolBar->setEnabled(true);
    }
}

MainWindow::~MainWindow() {
    // 确保正确释放资源
    if (image_label) delete image_label;
    if (statusBar) delete statusBar;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

void MainWindow::updateStatusBar(const std::string& data)
{
    statusBar->showMessage(QString::fromStdString(data));
}


// 添加图像更新槽函数
void MainWindow::updateImage(const QImage& image)
{
    // 检查图像是否有效且标签已初始化
    if (image_label && !image.isNull()) {
        // 计算左上角四分之一区域的大小
        int targetWidth = width() / 2;
        int targetHeight = height() / 2;

        // 缩放图像并保持比例
        QImage scaledImage = image.scaled(
            targetWidth, targetHeight,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );

        image_label->setPixmap(QPixmap::fromImage(scaledImage));
    }
}


}  // namespace class1_ros_qt_demo

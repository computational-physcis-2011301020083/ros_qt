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
#include <QVector3D>
#include <rviz/tool_manager.h>
#include <rviz/visualization_manager.h>
#include <rviz/render_panel.h>
#include <rviz/display.h>
#include <rviz/properties/property.h>
#include <rviz/default_plugin/view_controllers/orbit_view_controller.h>
#include "../include/ros_qt_demo/main_window.hpp"

namespace class1_ros_qt_demo {

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
    : QMainWindow(parent)
    , qnode(argc, argv)
{
    // 注册std::string类型，使其可以在信号槽中传递
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<QImage>("QImage");

    // 初始化中心部件和布局
    central_widget = new QWidget(this);
    setCentralWidget(central_widget);
    main_layout = new QHBoxLayout(central_widget);  // 主布局：左右各占1/2宽度

    // 左侧布局（放原图，占左半部分1/2宽度）
    left_layout = new QVBoxLayout();
    main_layout->addLayout(left_layout, 1);  // 权重1（总宽度的1/2）

    // 右侧布局（分上下两部分，各占1/2高度）
    right_layout = new QVBoxLayout();
    main_layout->addLayout(right_layout, 1);  // 权重1（总宽度的1/2）

    // 创建图像显示标签
    image_label = new QLabel(this);
    image_label->setText("Waiting for image...");
    //image_label->setAlignment(Qt::AlignCenter);
    image_label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    image_label->setStyleSheet("border: 1px solid black;");
    left_layout->addWidget(image_label, 1);  // 权重1，占左侧部分高度

    // RViz面板（右下角四分之一区域）
    rviz_panel = new rviz::RenderPanel();
    right_layout->addStretch(1);  // 上半部分留白（占右侧1/2高度）
    right_layout->addWidget(rviz_panel, 1);  // 下半部分放RViz（占右侧1/2高度）

    // 配置RViz
    rviz_frame = new rviz::VisualizationFrame();
    rviz_frame->initialize();
    rviz_manager = rviz_frame->getManager();
    rviz_manager ->initialize();
    rviz_manager ->startUpdate();

    // 绑定渲染面板到RViz管理器
    rviz_panel->initialize(rviz_manager->getSceneManager(), rviz_manager);

    // 设置RViz显示属性
    rviz_frame->setSplashPath("");
    rviz_frame->loadDisplayConfig("");
    rviz_frame->setMenuBar(nullptr);
    rviz_frame->setStatusBar(nullptr);
    rviz_frame->setHideButtonVisibility(false);

    // 添加URDF显示（修正后的代码）
    rviz::Display* robot_model_display = rviz_manager->createDisplay("rviz/RobotModel","Robot Model",true);
    if (robot_model_display) {
        robot_model_display->subProp("Robot Description")->setValue("robot_description");
        robot_model_display->subProp("Visual Enabled")->setValue(true);
        rviz::ViewManager* view_manager = rviz_manager->getViewManager();
        if (view_manager) {
            rviz_manager->setFixedFrame("base_link"); // 关键：固定参考系与URDF根坐标系一致
        }
        else {ROS_ERROR("Failed to create rviz view_manager!");}
    }
    else {ROS_ERROR("Failed to create RobotModel display!");
        return;
    }

    // 创建工具栏
    toolBar = addToolBar("ToolBar");

    // 创建状态栏
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    // 连接信号槽
    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
    QObject::connect(&qnode, SIGNAL(topicDataReceived(const std::string&)),this, SLOT(updateStatusBar(const std::string&)));
    QObject::connect(&qnode, SIGNAL(imageReceived(const QImage&)),this, SLOT(updateImage(const QImage&)));

    if (qnode.init()) {
      toolBar->setEnabled(true);
    }
}

MainWindow::~MainWindow() {
    // 确保正确释放资源
    //if (image_label) delete image_label;
    //if (statusBar) delete statusBar;
    //if (rviz_manager) delete rviz_manager;
    delete rviz_manager;
    delete rviz_panel;
    delete image_label;
    delete statusBar;
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

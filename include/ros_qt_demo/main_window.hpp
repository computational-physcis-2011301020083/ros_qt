/**
 * @file /include/class1_ros_qt_demo/main_window.hpp
 *
 * @brief Qt based gui for class1_ros_qt_demo.
 *
 * @date November 2010
 **/
#ifndef class1_ros_qt_demo_MAIN_WINDOW_H
#define class1_ros_qt_demo_MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"

#include <QtWidgets/QMainWindow>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>  // 用于显示图像
#include <QHBoxLayout>   // 水平布局
#include <QVBoxLayout>   // 垂直布局
#include <QWidget>
#include <QVector3D>
#include <QObject>  // 确保QObject头文件被包含
// RViz相关头文件
#include <rviz/visualization_frame.h>
#include <rviz/visualization_manager.h>
#include <rviz/render_panel.h>
#include <rviz/display.h>  // 补充Display类的完整定义
#include <rviz/view_manager.h>  // 补充ViewManager类的定义
#include <rviz/view_controller.h>
#include <rviz/default_plugin/view_controllers/orbit_view_controller.h>
#include "qnode.hpp"

namespace class1_ros_qt_demo {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(int argc, char** argv, QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

public Q_SLOTS:
    void updateStatusBar(const std::string& data);
    void updateImage(const QImage& image);  // 新增图像更新槽函数
    void on_actionAbout_triggered();
    void setupCameraPosition();
    void setupSceneLighting();

private:
    Ui::MainWindowDesign ui;

    QToolBar* toolBar;
    QStatusBar* statusBar;
    QNode qnode;
    QLabel* image_label;  // 图像显示标签

    // RViz相关组件
    rviz::RenderPanel* rviz_panel;          // RViz渲染面板
    rviz::VisualizationManager* rviz_manager; // RViz管理器
    rviz::VisualizationFrame* rviz_frame;

    // RViz布局管理器（用于划分界面区域）
    QWidget* central_widget;
    QHBoxLayout* main_layout;    // 主水平布局（分左右两部分）
    QVBoxLayout* left_layout;    // 左侧垂直布局（放原图）
    QVBoxLayout* right_layout;   // 右侧垂直布局（分上下两部分）

};

}  // namespace class1_ros_qt_demo

#endif // class1_ros_qt_demo_MAIN_WINDOW_H

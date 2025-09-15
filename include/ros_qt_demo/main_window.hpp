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
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>  // 用于显示图像
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

private:
    QToolBar* toolBar;
    QStatusBar* statusBar;
    QNode qnode;
    QLabel* image_label;  // 图像显示标签
};

}  // namespace class1_ros_qt_demo

#endif // class1_ros_qt_demo_MAIN_WINDOW_H

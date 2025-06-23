#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// 只需前向声明即可，避免头文件耦合
class ProTreeDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // 屏幕左侧的项目树对话框
    ProTreeDialog *protree_dialog_;

private slots:
    // 创建项目按钮的槽函数
    void createProject(bool);
    // 打开项目按钮的槽函数
    void openProject(bool);
};
#endif // MAINWINDOW_H

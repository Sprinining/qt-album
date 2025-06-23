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
private slots:
    void createPro(bool);
    void openPro(bool);

private:
    Ui::MainWindow *ui;
    ProTreeDialog *protree_dialog;
};
#endif // MAINWINDOW_H

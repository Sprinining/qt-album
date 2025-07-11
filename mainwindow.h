#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "picshowdialog.h"

#include <QMainWindow>
#include <QSplitter>

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

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    // 分裂器
    QSplitter *splitter_ = nullptr;
    // 屏幕左侧的项目树对话框
    ProTreeDialog *protree_dialog_ = nullptr;
    // 右侧的图片展示区
    PicShowDialog *pic_show_dialog_ = nullptr;
    QMenu *menu_file_ = nullptr;
    QMenu *menu_set_ = nullptr;
    QAction *action_create_pro_ = nullptr;
    QAction *action_open_pro_ = nullptr;
    QAction *action_music_ = nullptr;

    void initMenus();
    void initActions();
    void initSignals();
    void initDialog();

private slots:
    // 创建项目按钮的槽函数
    void onCreateProject(bool);
    // 打开项目按钮的槽函数
    void onOpenProject(bool);

signals:
    void openProject(QString dir_path);
};
#endif // MAINWINDOW_H

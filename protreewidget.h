#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include "protreethread.h"
#include <QAction>
#include <QProgressDialog>
#include <QTreeWidget>

class ProTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    explicit ProTreeWidget(QWidget *parent = nullptr);
    // 向树中添加一个项目节点，参数 name 为项目名称，dir_path 为项目所在目录
    void addProjectToTree(const QString &name, const QString &dir_path);

private:
    // 用于记录已经添加过的完整路径，防止重复添加
    QSet<QString> set_path_;
    QAction *action_import_ = nullptr;
    QAction *action_set_start_ = nullptr;
    QAction *action_close_pro_ = nullptr;
    QAction *action_slide_show_ = nullptr;
    ProTreeItem *right_clicked_item_ = nullptr;
    // 当前活动项目
    ProTreeItem *active_item_ = nullptr;
    QProgressDialog *dialog_progress_ = nullptr;
    std::shared_ptr<ProTreeThread> thread_create_pro_;

    void initActions();
    void initSignals();
    // 弹出目录选择对话框，返回用户选中的导入路径（若取消则返回空字符串）
    QString selectImportDirectory(const QString &initial_path);
    // 显示导入进度对话框，绑定取消按钮与相关处理逻辑
    void showProgressDialog();
    // 连接导入线程的信号与当前 ProTreeWidget 的槽函数
    void connectThreadSignals();

private slots:
    // 用户点击树控件中的某一项时触发（用于处理右键菜单等）
    void onItemPressed(QTreeWidgetItem *item, int column);
    // 用户右键选择“导入文件”动作时触发，启动导入目录线程
    void onImportProject();
    // 设置活动项目
    void onSetActiveProject();
    // 关闭项目
    void onCloseProject();
    // 启动幻灯片
    void onStartSlideshow();
    // 导入线程发出进度更新信号时调用，更新进度条显示（当前导入的文件数）
    void onProgressUpdated(int count);
    // 导入线程完成时触发，关闭并销毁进度条对话框
    void onProgressFinished();
    // 用户取消导入操作时触发，发出取消信号通知导入线程终止
    void onProgressCanceled();
    // 接收统计的文件总数
    void onTotalFileCountCalculated(int total);

signals:
    void progressCanceled();
};

#endif // PROTREEWIDGET_H

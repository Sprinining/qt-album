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
    QAction *action_import_;
    QAction *action_set_start_;
    QAction *action_close_pro_;
    QAction *action_slide_show_;
    ProTreeItem *right_clicked_item_;
    QProgressDialog *dialog_progress_;
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
    void onItemPressed(QTreeWidgetItem *item, int column);
    void onImportProject();
    void onProgressUpdated(int count);
    void onProgressFinished();
    void onProgressCanceled();

signals:
    void progressCanceled();
};

#endif // PROTREEWIDGET_H

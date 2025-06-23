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
    // 向树中添加一个项目节点，参数 name 为项目名称，path 为项目所在目录
    void addProjectToTree(const QString &name, const QString &path);

private:
    // 用于记录已经添加过的完整路径，防止重复添加
    QSet<QString> set_path_;
    QAction *act_import_;
    QAction *act_set_start_;
    QAction *act_close_pro_;
    QAction *act_slide_show_;
    ProTreeItem *right_btn_item_;
    QProgressDialog *dialog_progress_;
    std::shared_ptr<ProTreeThread> thread_create_pro_;

    void initActions();
    void initSignals();

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

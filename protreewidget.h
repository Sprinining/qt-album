#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include "protreethread.h"
#include <QAction>
#include <QProgressDialog>
#include <QTreeWidget>

class ProTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    // 构造函数，parent 是父级控件（通常用于界面嵌套）
    explicit ProTreeWidget(QWidget *parent = nullptr);
    // 向树中添加一个项目节点
    // 参数 name 为项目名称，path 为项目所在目录
    void addProToTree(const QString &name, const QString &path);

private slots:
    void onItemPressed(QTreeWidgetItem *item, int column);
    void import();
    void updateProgress(int count);
    void finishProgress();
    void canceled();

private:
    // 用于记录已经添加过的完整路径，防止重复添加
    QSet<QString> set_path;
    QAction *act_import;
    QAction *act_set_start;
    QAction *act_close_pro;
    QAction *act_slide_show;
    QTreeWidgetItem *right_btn_item;
    QProgressDialog *dialog_progress;
    std::shared_ptr<ProTreeThread> thread_create_pro_;

signals:
    void cancelProgress();
};

#endif // PROTREEWIDGET_H

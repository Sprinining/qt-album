#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include "protreeitem.h"
#include <QThread>
#include <QTreeWidget>
#include <QTreeWidgetItem>

// 项目图片导入构建线程（在后台递归构建图片目录树并更新 UI）
class ProTreeThread : public QThread {
    Q_OBJECT

public:
    ProTreeThread(const QString &src_path,    // 源路径
                  const QString &dist_path,   // 目标路径
                  ProTreeItem *parent_item,   // 父节点（用于添加子项）
                  int file_count,             // 初始文件计数
                  QTreeWidget *self,          // 树控件自身指针
                  ProTreeItem *root,          // 根节点
                  QObject *parent = nullptr); // Qt 所需父对象

    ~ProTreeThread(); // 析构函数

protected:
    void run() override; // 重写 QThread 的入口函数

private:
    void createProTree(
        const QString &src_path,          // 当前扫描的源路径
        const QString &dist_path,         // 当前目标路径
        ProTreeItem *parent_item,     // 当前目录对应的父节点
        int &file_count,                  // 累计图片数（引用传参）
        QTreeWidget *self,                // 指向树控件自身
        ProTreeItem *root,            // 根节点
        ProTreeItem *prev = nullptr); // 前一个兄弟节点，用于建立链表关系

    QString src_path_;             // 源路径
    QString dist_path_;            // 目标路径
    int file_count_;               // 文件计数
    ProTreeItem *parent_item_; // 父节点指针
    ProTreeItem *root_;        // 根节点指针
    QTreeWidget *self_;            // 树控件自身指针
    bool stop_ = false;            // 停止标志（用于取消操作）

public slots:
    void onProgressCanceled(); // 用户取消时调用

signals:
    void progressUpdated(int);  // 更新进度
    void progressFinished(int); // 完成构建
};

#endif // PROTREETHREAD_H

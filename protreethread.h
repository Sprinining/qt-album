#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include "protreeitem.h"
#include <QThread>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <atomic>

// 构造参数结构体
struct ProTreeThreadParams {
    QString src_path;                   // 导入的源目录路径
    QString dest_path;                  // 拷贝后的目标目录路径
    ProTreeItem *parent_item = nullptr; // 要插入子节点的父项目节点
    QTreeWidget *tree_widget = nullptr; // 指向当前树控件本体
    ProTreeItem *root = nullptr;        // 根节点（用于顶层构建或清理）
    AppConsts::ProjectOperation operation =
        AppConsts::ProjectOperation::Create; // 操作类型
};

class ProTreeThread : public QThread {
    Q_OBJECT

public:
    explicit ProTreeThread(const ProTreeThreadParams &params,
                           QObject *parent = nullptr);
    ~ProTreeThread() override;

protected:
    // QThread 的入口函数（线程逻辑写在这里）
    void run() override;

private:
    ProTreeThreadParams params_;   // 构造参数副本
    std::atomic_bool stop_{false}; // 停止标志（可由主线程触发取消操作）

    // 递归遍历目录并构建项目树
    void traverse(const QString &src_path,  // 当前遍历的源目录
                  const QString &dest_path, // 当前目标目录
                  ProTreeItem *parent_item, // 当前的父节点
                  int &file_count,          // 导入图片总数（引用统计）
                  ProTreeItem *root,        // 根节点（用于回退删除）
                  ProTreeItem *&prev);      // 前一个兄弟节点（构建链表）
    // 拷贝文件，如果需要的话
    bool copyIfNeeded(const QString &src, QString &dest);
    // 判断文件是否为支持的图片格式
    bool isValidImage(const QString &suffix) const;
    // 递归统计目录下所有图片数量
    int countImages(const QString &path) const;

public slots:
    // 外部调用，设置取消标志
    void onProgressCanceled();

signals:
    // 每导入一个文件触发，用于更新进度
    void progressUpdated(int count);
    // 全部导入完成触发（包含总文件数）
    void progressFinished(int total);
    // 每创建一个节点触发（主线程应响应此信号添加到 UI）
    // 线程中不操作 UI，UI 全部由主线程响应这个信号 来更新
    void itemCreated(ProTreeItem *parent, ProTreeItem *item);
    // 告知主线程最大值
    void totalFileCountCalculated(int);
};

#endif // PROTREETHREAD_H

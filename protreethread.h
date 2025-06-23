#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidgetItem>

// 项目图片导入构建线程（递归构建图片目录树，并更新 UI）
class ProTreeThread : public QThread {
    Q_OBJECT

public:
    // 构造函数：初始化路径、树结构信息、计数器等
    ProTreeThread(const QString &src_path,      // 源路径
                  const QString &dist_path,     // 目标路径
                  QTreeWidgetItem *parent_item, // 父节点（用于添加子项）
                  int file_count,               // 初始文件计数
                  QTreeWidget *self,            // 树控件自身指针
                  QTreeWidgetItem *root,        // 根节点
                  QObject *parent = nullptr);   // Qt 所需父对象

    ~ProTreeThread();

protected:
    // 重写 run()：线程入口函数
    void run() override;

private:
    // 递归创建项目树
    void createProTree(const QString &src_path, const QString &dist_path,
                       QTreeWidgetItem *parent_item, int &file_count,
                       QTreeWidget *self, QTreeWidgetItem *root,
                       QTreeWidgetItem *prev = nullptr);

    // 成员变量（缓存参数）
    QString src_path_;
    QString dist_path_;
    int file_count_;
    QTreeWidgetItem *parent_item_;
    QTreeWidgetItem *root_;
    QTreeWidget *self_;
    bool stop_ = false;

public slots:
    void cancelProgress();

signals:
    void updateProgress(int); // 发送进度更新信号
    void finishProgress(int); // 发送完成信号
};

#endif // PROTREETHREAD_H

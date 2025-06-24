#include "protreethread.h"
#include "consts.h"
#include <QDebug>
#include <QDir>
#include <QFile>

ProTreeThread::ProTreeThread(const ProTreeThreadParams &params, QObject *parent)
    : QThread(parent), params_(params) {}

ProTreeThread::~ProTreeThread() {}

void ProTreeThread::run() {
    int total_count = countImages(params_.src_path);
    emit progressUpdated(0);
    emit totalFileCountCalculated(total_count);

    int file_count = 0;
    // 用于链表式连接同一层的兄弟节点，初始为空，递归过程中更新
    ProTreeItem *prev = nullptr;

    // 启动目录遍历构建逻辑
    traverse(params_.src_path, params_.dest_path, params_.parent_item, file_count,
             params_.root, prev);

    // 若线程被取消，回滚删除节点和文件夹
    if (stop_) {
        // 1. 从树控件中查找这个项目节点的 index
        auto index = params_.tree_widget->indexOfTopLevelItem(params_.root);

        // 2. 从树控件中删除该节点（释放 UI 资源）
        delete params_.tree_widget->takeTopLevelItem(index);

        // 3. 删除对应的磁盘目录（包含已复制文件）
        QDir(params_.root->getFilePath()).removeRecursively();

        return;
    }

    // 发出完成信号，包含图片总数
    emit progressFinished(file_count);
}

// 递归遍历目录，构建对应的项目树节点，并根据需要拷贝文件
void ProTreeThread::traverse(const QString &src_path, const QString &dest_path,
                             ProTreeItem *parent_item, int &file_count,
                             ProTreeItem *root, ProTreeItem *&prev) {
    if (stop_)
        return;

    // 获取该目录下符合过滤条件的文件信息列表，包括文件和子目录
    QDir dir(src_path);
    // 设置过滤规则：只遍历目录和文件，排除 "." 和 ".." 特殊目录
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    // 获取该目录下符合过滤条件的文件信息列表，包括文件和子目录
    const QFileInfoList &list = dir.entryInfoList();

    // 遍历该目录下的每一个文件或文件夹信息
    for (const QFileInfo &info : list) {
        if (stop_)
            return;
        // 取文件或目录名
        const QString name = info.fileName();
        // 获取源目录下该文件或目录的绝对路径
        const QString abs_src_path = info.absoluteFilePath();
        // 计算目标目录下对应的绝对路径
        QString abs_dst_path = QDir(dest_path).absoluteFilePath(name);

        // 如果是目录，递归处理
        if (info.isDir()) {
            QDir dst(abs_dst_path);
            // 若目标目录不存在，则尝试创建
            if (!dst.exists() && !dst.mkpath(abs_dst_path)) {
                qWarning() << "Failed to create dir:" << abs_dst_path;
                continue;
            }

            // 创建表示该目录的项目树节点（目录类型）
            // 注意：不直接操作 UI，UI 线程通过 itemCreated 信号接收节点
            auto *item = new ProTreeItem(parent_item, name, abs_dst_path, root,
                                         AppConsts::TreeItemType::Directory);
            // 设置节点显示名称、图标、鼠标悬停提示
            item->setData(0, Qt::DisplayRole, name);
            item->setData(0, Qt::DecorationRole, QIcon(":/icons/dir.png"));
            item->setData(0, Qt::ToolTipRole, abs_dst_path);

            // 通过信号通知主线程，将该目录节点添加到树控件中
            emit itemCreated(parent_item, item);

            // 递归调用遍历子目录，构建子树
            traverse(abs_src_path, abs_dst_path, item, file_count, root, prev);
        } else {
            // 当前是文件，先检查文件后缀是否为合法图片格式
            const QString suffix = info.completeSuffix().toLower();
            if (!isValidImage(suffix))
                continue;

            // 根据源目录和目标目录判断是否需要拷贝文件
            if (!copyIfNeeded(abs_src_path, abs_dst_path))
                continue;

            // 成功导入一个图片文件，计数器加一
            file_count++;
            // 发射进度更新信号，通知主线程刷新进度条等 UI
            emit progressUpdated(file_count);

            // 创建表示该图片文件的项目树节点（图片类型）
            auto *item = new ProTreeItem(parent_item, name, abs_dst_path, root,
                                         AppConsts::TreeItemType::Picture);
            item->setData(0, Qt::DisplayRole, name);
            item->setData(0, Qt::DecorationRole, QIcon(":/icons/pic.png"));
            item->setData(0, Qt::ToolTipRole, abs_dst_path);

            // 维护兄弟节点链表结构：将当前节点的前驱设置为 prev
            item->setItemPrev(prev);
            // 如果 prev 不为空，将其后继设置为当前节点，实现双向链表
            if (prev)
                prev->setItemNext(item);
            // 更新 prev，指向当前节点，为下一次循环做准备
            prev = item;

            // 通过信号通知主线程，将该图片节点添加到树控件中
            emit itemCreated(parent_item, item);
        }
    }
}

// 拷贝文件（如果源路径与目标路径不同）
bool ProTreeThread::copyIfNeeded(const QString &src, QString &dest) {
    if (params_.src_path == params_.dest_path)
        return true; // 同目录无需拷贝

    if (QFile::exists(dest))
        QFile::remove(dest); // 移除目标已有文件

    if (!QFile::copy(src, dest)) {
        qWarning() << "Copy failed:" << src << "->" << dest;
        return false;
    }

    return true;
}

// 检查是否是支持的图片格式
bool ProTreeThread::isValidImage(const QString &suffix) const {
    static const QSet<QString> image_types = {"png", "jpg", "jpeg"};
    return image_types.contains(suffix);
}

// 递归统计目录下所有图片文件数量（包括子目录）
int ProTreeThread::countImages(const QString &path) const {
    int count = 0;

    QDir dir(path);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    const QFileInfoList &list = dir.entryInfoList();

    for (const QFileInfo &info : list) {
        if (info.isDir()) {
            // 递归处理子目录
            count += countImages(info.absoluteFilePath());
        } else {
            // 判断是否为合法图片后缀
            const QString suffix = info.completeSuffix().toLower();
            if (isValidImage(suffix))
                ++count;
        }
    }

    return count;
}

// 设置线程终止标志
void ProTreeThread::onProgressCanceled() { stop_ = true; }

#include "protreethread.h"
#include "consts.h"
#include "protreeitem.h"
#include <QDebug>
#include <QDir>
#include <QFile>

ProTreeThread::ProTreeThread(const QString &src_path, const QString &dist_path,
                             ProTreeItem *parent_item, int file_count,
                             QTreeWidget *self, ProTreeItem *root,
                             QObject *parent)
    : QThread(parent), src_path_(src_path), dist_path_(dist_path),
      parent_item_(parent_item), file_count_(file_count), self_(self),
      root_(root), stop_(false) {}

ProTreeThread::~ProTreeThread() {}

void ProTreeThread::run() {
    qDebug() << "[Thread] ProTreeThread::run 开始构建项目树";

    createProTree(src_path_, dist_path_, parent_item_, file_count_, self_, root_);

    if (stop_) {
        auto path = dynamic_cast<ProTreeItem *>(root_)->getPath();
        auto index = self_->indexOfTopLevelItem(root_);
        delete self_->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

    // 线程结束后发送完成信号
    emit progressFinished(file_count_);
}

void ProTreeThread::createProTree(const QString &src_path,
                                  const QString &dist_path,
                                  ProTreeItem *parent_item, int &file_count,
                                  QTreeWidget *self, ProTreeItem *root,
                                  ProTreeItem *prev) {
    if (stop_)
        return;

    bool need_copy = (src_path != dist_path); // 是否需要拷贝文件

    QDir import_dir(src_path);
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    import_dir.setSorting(QDir::Name);

    QFileInfoList list = import_dir.entryInfoList();

    for (const QFileInfo &file_info : list) {
        if (stop_)
            return;

        if (file_info.isDir()) {
            // 可拓展递归子目录逻辑（目前未处理子目录）

            if (stop_)
                return;
            emit progressUpdated(file_count);
            QDir dist_dir(dist_path_);
            QString sub_dist_path = dist_dir.absoluteFilePath(file_info.fileName());
            QDir sub_dist_dir(sub_dist_path);
            if (!sub_dist_dir.exists()) {
                bool ok = sub_dist_dir.mkpath(sub_dist_path);
                if (!ok)
                    continue;
            }
            auto *item =
                new ProTreeItem(parent_item, file_info.fileName(), sub_dist_path,
                                         root, AppConsts::TreeItemType::Directory);
            item->setData(0, Qt::DisplayRole, file_info.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icons/dir.png"));
            item->setData(0, Qt::ToolTipRole, sub_dist_path);

            createProTree(file_info.absoluteFilePath(), sub_dist_path, item,
                          file_count, self, root, prev);

            continue;
        }

        // 判断文件后缀是否合法（只接受图片）
        const QString suffix = file_info.completeSuffix().toLower();
        if (suffix != "png" && suffix != "jpg" && suffix != "jpeg")
            continue;

        file_count++;
        emit progressUpdated(file_count); // 发射进度信号

        QString dist_file_path = file_info.absoluteFilePath();

        // 如果需要拷贝，则执行拷贝到 dist_path 下
        if (need_copy) {
            QDir dist_dir(dist_path);
            dist_file_path = dist_dir.absoluteFilePath(file_info.fileName());
            if (!QFile::copy(file_info.absoluteFilePath(), dist_file_path)) {
                qWarning() << "Failed to copy from" << file_info.absoluteFilePath()
                           << "to" << dist_file_path;
                continue;
            }
        }

        // 创建图片项并挂到树中
        auto *item =
            new ProTreeItem(parent_item, file_info.fileName(), dist_file_path, root,
                                     AppConsts::TreeItemType::Picture);

        item->setData(0, Qt::DisplayRole, file_info.fileName());
        item->setData(0, Qt::DecorationRole, QIcon(":/icons/pic.png"));
        item->setData(0, Qt::ToolTipRole, dist_file_path);

        // 设置链表前后关系（可用于双向遍历）
        if (prev) {
            auto *prev_pro_item = dynamic_cast<ProTreeItem *>(prev);
            if (prev_pro_item) {
                prev_pro_item->setItemNext(item);
            }
        }

        item->setItemPrev(item); // 当前节点标记自己为前驱（可能是用于链表起点）
        prev = item;
    }

    // 保留：后续可添加 name_filters 判断或递归目录构建
}

void ProTreeThread::onProgressCanceled() { this->stop_ = true; }

#pragma once
#include "consts.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

// 项目树（ProTreeWidget）中的一个节点项
class ProTreeItem : public QTreeWidgetItem {
public:
    // 创建顶层节点：指定树控件指针 treeview、节点显示名称 name、路径 file_path
    // 和节点类型 type
    ProTreeItem(QTreeWidget *treeview, const QString &name,
                const QString &file_path,
                AppConsts::TreeItemType type = AppConsts::TreeItemType::Project);

    // 创建子节点：指定父节点、节点显示名称、路径、根节点指针和节点类型
    ProTreeItem(ProTreeItem *parent, const QString &name,
                const QString &file_path, ProTreeItem *root,
                AppConsts::TreeItemType type = AppConsts::TreeItemType::Project);

    const QString &getFilePath() const;
    const QTreeWidgetItem *getRoot() const;
    const ProTreeItem *getPrevItem() const;
    const ProTreeItem *getNextItem() const;
    // 返回当前项目里的第一个图片 item
    const ProTreeItem *getFirstPicItem() const;
    const ProTreeItem *getLastPicItem() const;
    AppConsts::ProjectSource getSource() const;

    void setItemPrev(ProTreeItem *prev);
    void setItemNext(ProTreeItem *next);
    void setSource(AppConsts::ProjectSource source);

    void setFirstPicItem(ProTreeItem *item);
    void setLastPicItem(ProTreeItem *item);

private:
    QString file_path_; // 节点对应的路径
    QString name_;      // 节点名称
    ProTreeItem *item_root_ = nullptr;
    ProTreeItem *item_prev_ = nullptr;
    ProTreeItem *item_next_ = nullptr;
    ProTreeItem *item_pic_first_ = nullptr;
    ProTreeItem *item_pic_last_ = nullptr;
    // 项目源是导入还是创建的
    AppConsts::ProjectSource source_ = AppConsts::ProjectSource::Created;
};

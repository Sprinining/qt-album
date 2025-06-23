#pragma once
#include "consts.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

// 项目树（ProTreeWidget）中的一个节点项
class ProTreeItem : public QTreeWidgetItem {
public:
    // 创建顶层节点：指定树控件指针 treeview、节点显示名称 name、路径 file_path
    // 和节点类型 type
    ProTreeItem(QTreeWidget *treeview, const QString &name, const QString &file_path,
                AppConsts::TreeItemType type = AppConsts::TreeItemType::Project);

    // 创建子节点：指定父节点、节点显示名称、路径、根节点指针和节点类型
    ProTreeItem(ProTreeItem *parent, const QString &name, const QString &file_path,
                ProTreeItem *root,
                AppConsts::TreeItemType type = AppConsts::TreeItemType::Project);

    const QString &getFilePath() const;
    const QTreeWidgetItem *getRoot() const;
    const ProTreeItem *getItemPrev() const;
    const ProTreeItem *getItemNext() const;
    const ProTreeItem *getFirstItemChild() const;
    const ProTreeItem *getLastItemChild() const;

    void setItemPrev(ProTreeItem *prev);
    void setItemNext(ProTreeItem *next);

private:
    QString file_path_; // 节点对应的路径
    QString name_; // 节点名称
    ProTreeItem *item_root_;
    ProTreeItem *item_prev_;
    ProTreeItem *item_next_;
};

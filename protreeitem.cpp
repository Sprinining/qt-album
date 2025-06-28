#include "protreeitem.h"

// 调用基类构造，treeview 是父对象，节点自动加入树中
ProTreeItem::ProTreeItem(QTreeWidget *treeview, const QString &name,
                         const QString &file_path, AppConsts::TreeItemType type)
    : QTreeWidgetItem(treeview, static_cast<int>(type)), file_path_(file_path),
    name_(name), item_root_(this) {}
// 父节点parent作为父对象，加入父节点的子列表
ProTreeItem::ProTreeItem(ProTreeItem *parent, const QString &name,
                         const QString &file_path, ProTreeItem *root,
                         AppConsts::TreeItemType type)
    : QTreeWidgetItem(parent, static_cast<int>(type)), file_path_(file_path),
    name_(name), item_root_(root) {}

const QString &ProTreeItem::getFilePath() const { return file_path_; }

const QTreeWidgetItem *ProTreeItem::getRoot() const { return item_root_; }

const ProTreeItem *ProTreeItem::getPrevItem() const { return item_prev_; }

const ProTreeItem *ProTreeItem::getNextItem() const { return item_next_; }

const ProTreeItem *ProTreeItem::getFirstPicItem() const {
    return item_pic_first_;
}

const ProTreeItem *ProTreeItem::getLastPicItem() const {
    return item_pic_last_;
}

AppConsts::ProjectSource ProTreeItem::getSource() const { return source_; }

void ProTreeItem::setItemPrev(ProTreeItem *prev) { item_prev_ = prev; }

void ProTreeItem::setItemNext(ProTreeItem *next) { item_next_ = next; }

void ProTreeItem::setSource(AppConsts::ProjectSource source) {
    source_ = source;
}

void ProTreeItem::setFirstPicItem(ProTreeItem *item) { item_pic_first_ = item; }

void ProTreeItem::setLastPicItem(ProTreeItem *item) { item_pic_last_ = item; }

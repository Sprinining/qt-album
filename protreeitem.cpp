#include "protreeitem.h"

// 调用基类构造，treeview 是父对象，节点自动加入树中
ProTreeItem::ProTreeItem(QTreeWidget* treeview, const QString& name, const QString& path, int type)
    : QTreeWidgetItem(treeview, type), path(path), name(name), item_root(this), item_prev(nullptr), item_next(nullptr) {
}
// 父节点parent作为父对象，加入父节点的子列表
ProTreeItem::ProTreeItem(QTreeWidgetItem* parent, const QString& name, const QString& path, QTreeWidgetItem* root, int type)
    : QTreeWidgetItem(parent, type), path(path), name(name), item_root(root), item_prev(nullptr), item_next(nullptr) {
}

const QString& ProTreeItem::getPath() const {
    return path;
}

const QTreeWidgetItem* ProTreeItem::getRoot() const {
    return item_root;
}

const ProTreeItem* ProTreeItem::getItemPrev() const {
    return dynamic_cast<ProTreeItem*>(item_prev);
}

const ProTreeItem* ProTreeItem::getItemNext() const {
    return dynamic_cast<ProTreeItem*>(item_next);
}

const ProTreeItem* ProTreeItem::getFirstItemChild() const {
}

const ProTreeItem* ProTreeItem::getLastItemChild() const {
}

void ProTreeItem::setItemPrev(ProTreeItem* prev) {
    item_prev = prev;
}

void ProTreeItem::setItemNext(ProTreeItem* next) {
    item_next = next;
}

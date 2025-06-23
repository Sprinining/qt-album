#include "protreeitem.h"

// 调用基类构造，treeview 是父对象，节点自动加入树中
ProTreeItem::ProTreeItem(QTreeWidget *treeview, const QString &name,
                         const QString &path, AppConsts::TreeItemType type)
    : QTreeWidgetItem(treeview, static_cast<int>(type)), path_(path),
    name_(name), item_root_(this), item_prev_(nullptr), item_next_(nullptr) {}
// 父节点parent作为父对象，加入父节点的子列表
ProTreeItem::ProTreeItem(ProTreeItem *parent, const QString &name,
                         const QString &path, ProTreeItem *root,
                         AppConsts::TreeItemType type)
    : QTreeWidgetItem(parent, static_cast<int>(type)), path_(path), name_(name),
    item_root_(root), item_prev_(nullptr), item_next_(nullptr) {}

const QString &ProTreeItem::getPath() const { return path_; }

const QTreeWidgetItem *ProTreeItem::getRoot() const { return item_root_; }

const ProTreeItem *ProTreeItem::getItemPrev() const { return item_prev_; }

const ProTreeItem *ProTreeItem::getItemNext() const { return item_next_; }

// todo: 函数定义待完善
const ProTreeItem *ProTreeItem::getFirstItemChild() const {}

const ProTreeItem *ProTreeItem::getLastItemChild() const {}

void ProTreeItem::setItemPrev(ProTreeItem *prev) { item_prev_ = prev; }

void ProTreeItem::setItemNext(ProTreeItem *next) { item_next_ = next; }

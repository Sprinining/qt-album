#include "protreeitem.h"

ProTreeItem::ProTreeItem(QTreeWidget* treeview, const QString& name, const QString& path, int type)
    : QTreeWidgetItem(treeview, type) // 调用基类构造，treeview 是父对象，节点自动加入树中
{
}

ProTreeItem::ProTreeItem(QTreeWidgetItem* parent, const QString& name, const QString& path, QTreeWidgetItem* root, int type)
    : QTreeWidgetItem(parent, type) // 父节点parent作为父对象，加入父节点的子列表
{
}

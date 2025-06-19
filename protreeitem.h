#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

// 自定义的树节点类，用于在项目树（ProTreeWidget）中表示一个项目项或子项
class ProTreeItem : public QTreeWidgetItem {
  public:
    /**
     * @brief 构造函数：创建一个根节点（顶级项）
     * @param treeview 所属的 QTreeWidget 指针
     * @param name 项目名称或显示文本
     * @param path 项目路径，用于标识或存储数据
     * @param type 节点类型（可自定义类型常量，用于区分不同类型节点）
     */
    ProTreeItem(QTreeWidget* treeview, const QString& name, const QString& path, int type = Type);

    /**
     * @brief 构造函数：创建一个子节点（有父项）
     * @param parent 父节点（QTreeWidgetItem 指针）
     * @param name 项目名称或显示文本
     * @param path 项目路径
     * @param root 根节点（如果需要建立父子关系链，可选参数）
     * @param type 节点类型
     */
    ProTreeItem(QTreeWidgetItem* parent, const QString& name, const QString& path, QTreeWidgetItem* root, int type = Type);
};

#endif // PROTREEITEM_H

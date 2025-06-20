#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

// 自定义树节点类：表示项目树（ProTreeWidget）中的一个节点项
class ProTreeItem : public QTreeWidgetItem {
  public:
    // 创建顶层节点，指定树控件指针、节点显示名称、路径和节点类型
    ProTreeItem(QTreeWidget* treeview, const QString& name, const QString& path, int type = Type);

    // 创建子节点，指定父节点、节点显示名称、路径、根节点指针和节点类型
    ProTreeItem(QTreeWidgetItem* parent, const QString& name, const QString& path, QTreeWidgetItem* root, int type = Type);

    const QString& getPath() const;               // 获取该节点关联的路径
    const QTreeWidgetItem* getRoot() const;       // 获取根节点指针（根节点可用于查找或定位项目树起点）
    const ProTreeItem* getItemPrev() const;       // 获取前一个兄弟节点（同级上一个节点）
    const ProTreeItem* getItemNext() const;       // 获取下一个兄弟节点（同级下一个节点）
    const ProTreeItem* getFirstItemChild() const; // 获取第一个子节点
    const ProTreeItem* getLastItemChild() const;  // 获取最后一个子节点

    void setItemPrev(ProTreeItem* prev); // 设置前一个兄弟节点
    void setItemNext(ProTreeItem* next); // 设置下一个兄弟节点

  private:
    QString path;               // 节点对应的路径（通常用于表示文件/目录路径）
    QString name;               // 节点名称（可用于显示或标识，当前未使用）
    QTreeWidgetItem* item_root; // 根节点指针（记录整棵树的顶层节点）
    QTreeWidgetItem* item_prev; // 前一个兄弟节点（用于链表式遍历或自定义导航）
    QTreeWidgetItem* item_next; // 下一个兄弟节点（同上）
};

#endif // PROTREEITEM_H

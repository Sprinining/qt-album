#include "protreewidget.h"
#include "consts.h"
#include "protreeitem.h"
#include <QDir>

// 传给基类构造函数
ProTreeWidget::ProTreeWidget(QWidget* parent) : QTreeWidget(parent) {
}

// 添加一个项目节点到树形控件中
void ProTreeWidget::addProToTree(const QString& name, const QString& path) {
    // 拼接绝对路径（完整项目路径 = 路径 + 项目名）
    QDir dir(path);
    const QString file_path = dir.absoluteFilePath(name);

    // 如果已经添加过该路径的项目，避免重复添加
    if (set_path.contains(file_path)) return;

    // 如果目录不存在，尝试创建（确保物理路径存在）
    QDir pro_dir(file_path);
    if (!pro_dir.exists()) {
        if (!pro_dir.mkpath(".")) return; // 创建失败直接返回
    }

    // 记录该项目路径，防止重复
    set_path.insert(file_path);

    // 创建自定义项目节点并添加到树上（以此树控件为父）
    auto* item = new ProTreeItem(this, name, file_path, TreeItemPro);

    // 设置显示文本（第0列）
    item->setData(0, Qt::DisplayRole, name);

    // 设置图标装饰（比如项目图标）
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));

    // 设置鼠标悬停提示
    item->setData(0, Qt::ToolTipRole, file_path);
}

#include "protreewidget.h"
#include "consts.h"
#include "protreeitem.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenu>

// 传给基类构造函数
ProTreeWidget::ProTreeWidget(QWidget* parent) : QTreeWidget(parent) {
    // 当用户点击任意 item 时，触发 onItemPressed 处理（用于弹出右键菜单等）
    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::onItemPressed);

    // 导入文件动作
    act_import = new QAction(QIcon(":/icons/import.png"), tr("导入文件"), this);
    connect(act_import, &QAction::triggered, this, &ProTreeWidget::import);

    // 设置活动项目
    act_set_start = new QAction(QIcon(":/icons/core.png"), tr("设置活动项目"), this);
    // connect(act_set_start, &QAction::triggered, this, &ProTreeWidget::setAsActive);

    // 关闭项目
    act_close_pro = new QAction(QIcon(":/icons/close.png"), tr("关闭项目"), this);
    // connect(act_close_pro, &QAction::triggered, this, &ProTreeWidget::closeProject);

    // 幻灯片播放
    act_slide_show = new QAction(QIcon(":/icons/slideshow.png"), tr("轮播图播放"), this);
    // connect(act_slide_show, &QAction::triggered, this, &ProTreeWidget::startSlideshow);
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

    // 设置显示文本（第0列）s
    item->setData(0, Qt::DisplayRole, name);

    // 设置图标装饰（比如项目图标）
    item->setData(0, Qt::DecorationRole, QIcon(":/icons/dir.png"));

    // 设置鼠标悬停提示
    item->setData(0, Qt::ToolTipRole, file_path);
}

void ProTreeWidget::onItemPressed(QTreeWidgetItem* item, int column) {
    // 检查是否为右键点击（仅处理右键）
    if (QGuiApplication::mouseButtons() != Qt::RightButton) return;

    // 空指针检查，防止异常
    if (!item) return;

    // 只处理自定义项目节点（类型为 TreeItemPro）
    if (item->type() != TreeItemPro) return;

    // 记录当前右键点击的项（用于后续槽函数中定位上下文）
    right_btn_item = item;

    // 创建右键菜单，并添加对应的操作项
    QMenu contextMenu(this);
    contextMenu.addAction(act_import);     // 导入文件
    contextMenu.addAction(act_set_start);  // 设置为起始项目
    contextMenu.addAction(act_close_pro);  // 关闭项目
    contextMenu.addAction(act_slide_show); // 幻灯片浏览

    // 在鼠标位置弹出菜单（阻塞直到用户操作）
    contextMenu.exec(QCursor::pos());
}

void ProTreeWidget::import() {
    // 安全检查：未选中任何节点时直接返回，并使用默认路径
    if (!right_btn_item) return;

    // 获取当前项目节点路径
    ProTreeItem* proItem = dynamic_cast<ProTreeItem*>(right_btn_item);
    if (!proItem) return;

    const QString basePath = proItem->getPath();

    // 构造文件夹选择对话框
    QFileDialog fileDialog(this);                      // 加 this 作为父窗口，防止窗口异常
    fileDialog.setFileMode(QFileDialog::Directory);    // 只允许选择文件夹
    fileDialog.setWindowTitle(tr("选择导入的文件夹")); // 设置窗口标题
    fileDialog.setDirectory(basePath);                 // 设置初始目录
    fileDialog.setViewMode(QFileDialog::Detail);       // 设置详细视图（非图标模式）

    // 获取用户选择结果
    QStringList selectedDirs;
    if (fileDialog.exec()) selectedDirs = fileDialog.selectedFiles();

    // 用户取消或未选择目录
    if (selectedDirs.isEmpty()) return;

    // 获取第一个选中的路径
    const QString importPath = selectedDirs.at(0);

    qDebug() << importPath;
}

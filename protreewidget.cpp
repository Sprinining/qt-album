#include "protreewidget.h"
#include "consts.h"
#include "protreeitem.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenu>

// 传给基类构造函数
ProTreeWidget::ProTreeWidget(QWidget *parent) : QTreeWidget(parent) {
    // 只初始化一次，节省资源，可以多次复用
    initActions();
    initSignals();
}

// 添加一个项目节点到树形控件中
void ProTreeWidget::addProjectToTree(const QString &name, const QString &path) {
    // 拼接绝对路径（完整项目路径 = 路径 + 项目名）
    const QString file_path = QDir(path).absoluteFilePath(name);

    // 避免重复添加
    if (set_path_.contains(file_path))
        return;

    // 如果目录不存在，尝试创建（确保物理路径存在）
    QDir pro_dir(file_path);
    if (!pro_dir.exists() && !pro_dir.mkpath("."))
        return;

    // 加到 set 中，防止重复
    set_path_.insert(file_path);

    // 创建根节点（以此树控件为父）
    auto *item =
        new ProTreeItem(this, name, file_path, AppConsts::TreeItemType::Project);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icons/dir.png"));
    item->setData(0, Qt::ToolTipRole, file_path);
}

void ProTreeWidget::initActions() {
    action_import_ =
        new QAction(QIcon(":/icons/import.png"), tr("导入文件"), this);
    action_set_start_ =
        new QAction(QIcon(":/icons/core.png"), tr("设置活动项目"), this);
    action_close_pro_ =
        new QAction(QIcon(":/icons/close.png"), tr("关闭项目"), this);
    action_slide_show_ =
        new QAction(QIcon(":/icons/slideshow.png"), tr("幻灯片播放"), this);
}

void ProTreeWidget::initSignals() {
    // 当用户点击任意 item 时，触发 onItemPressed 处理（用于弹出右键菜单等）
    connect(this, &ProTreeWidget::itemPressed, this,
            &ProTreeWidget::onItemPressed);

    connect(action_import_, &QAction::triggered, this,
            &ProTreeWidget::onImportProject);

    // connect(act_set_start, &QAction::triggered, this,
    // &ProTreeWidget::setAsActive);

    // connect(act_close_pro, &QAction::triggered, this,
    // &ProTreeWidget::closeProject);

    // connect(act_slide_show, &QAction::triggered, this,
    // &ProTreeWidget::startSlideshow);
}

void ProTreeWidget::onItemPressed(QTreeWidgetItem *item, int column) {
    // 检查是否为右键点击（仅处理右键）
    if (QGuiApplication::mouseButtons() != Qt::RightButton)
        return;

    // 空指针检查，防止异常
    if (!item)
        return;

    // 只处理自定义项目节点（类型为 TreeItemPro）
    if (item->type() != static_cast<int>(AppConsts::TreeItemType::Project))
        return;

    // 记录当前右键点击的项（用于后续槽函数中定位上下文）
    right_clicked_item_ = dynamic_cast<ProTreeItem *>(item);

    // 创建右键菜单，并添加对应的操作项
    QMenu menu(this);
    menu.addAction(action_import_);     // 导入文件
    menu.addAction(action_set_start_);  // 设置为起始项目
    menu.addAction(action_close_pro_);  // 关闭项目
    menu.addAction(action_slide_show_); // 幻灯片浏览

    // 在鼠标位置弹出菜单（阻塞直到用户操作）
    menu.exec(QCursor::pos());
}

void ProTreeWidget::onImportProject() {
    // 获取当前右键点击的项目节点
    ProTreeItem *project_item = right_clicked_item_;
    if (!project_item)
        return;

    // 该节点的路径作为导入的目标路径
    const QString dest_path = project_item->getFilePath();

    // 弹出目录选择对话框，获取用户选择的导入路径
    const QString src_path = selectImportDirectory(dest_path);
    if (src_path.isEmpty())
        return;

    // 创建导入线程对象，传入导入路径、目标路径、当前节点等参数
    ProTreeThreadParams params{.src_path = src_path,
                               .dest_path = dest_path,
                               .parent_item = project_item,
                               .tree_widget = this,
                               .root = project_item};

    thread_create_pro_ = std::make_shared<ProTreeThread>(params);

    // 连接导入线程的相关信号（进度更新、完成、取消等）
    connectThreadSignals();

    // 启动导入线程，开始执行导入任务
    thread_create_pro_->start();

    // 显示导入进度对话框，支持取消操作
    showProgressDialog();
}

QString ProTreeWidget::selectImportDirectory(const QString &initial_path) {

    // 构造文件夹选择对话框
    QFileDialog file_dialog(this); // 加 this 作为父窗口，防止窗口异常
    file_dialog.setFileMode(QFileDialog::Directory);    // 只允许选择文件夹
    file_dialog.setWindowTitle(tr("选择导入的文件夹")); // 设置窗口标题
    file_dialog.setDirectory(initial_path);             // 设置初始目录
    file_dialog.setViewMode(QFileDialog::Detail); // 设置详细视图（非图标模式）

    if (file_dialog.exec())
        return file_dialog.selectedFiles().value(0);

    return QString();
}

void ProTreeWidget::connectThreadSignals() {
    // 获取线程指针，方便后续信号连接
    auto *thread = thread_create_pro_.get();

    // 连接线程的 itemCreated 信号到一个 lambda 槽函数
    // 每当线程创建一个新的 ProTreeItem 时，主线程收到信号，
    // 将这个新节点添加为其父节点的子节点，从而在 UI 树中动态构建树形结构
    connect(thread, &ProTreeThread::itemCreated, this,
            [](ProTreeItem *parent, ProTreeItem *item) {
        if (parent && item)       // 确保指针有效
            parent->addChild(item); // 将新节点插入父节点
    });

    // 实时刷新 UI 上的进度显示
    connect(thread, &ProTreeThread::progressUpdated, this,
            &ProTreeWidget::onProgressUpdated);

    // 更新 UI 状态，关闭进度对话框等
    connect(thread, &ProTreeThread::progressFinished, this,
            &ProTreeWidget::onProgressFinished);

    // 用户点击取消时，触发此信号通知线程停止操作，实现线程安全取消
    connect(this, &ProTreeWidget::progressCanceled, thread,
            &ProTreeThread::onProgressCanceled);
}

void ProTreeWidget::showProgressDialog() {
    dialog_progress_ = new QProgressDialog(this);
    dialog_progress_->setWindowTitle(tr("请等待..."));
    dialog_progress_->setFixedWidth(AppConsts::UIConfig::ProgressWidth);
    dialog_progress_->setRange(0, AppConsts::UIConfig::ProgressWidth);

    connect(dialog_progress_, &QProgressDialog::canceled, this,
            &ProTreeWidget::onProgressCanceled);

    dialog_progress_->exec();
}

void ProTreeWidget::onProgressUpdated(int count) {
    if (!dialog_progress_)
        return;
    dialog_progress_->setMaximum(
        AppConsts::UIConfig::ProgressMax); // 或任意足够大的数
    dialog_progress_->setValue(count);
}

void ProTreeWidget::onProgressFinished() {
    dialog_progress_->setValue(AppConsts::UIConfig::ProgressMax);
    dialog_progress_->deleteLater();
    dialog_progress_ = nullptr;

    // 释放线程对象
    thread_create_pro_.reset();
}

void ProTreeWidget::onProgressCanceled() {
    emit progressCanceled();
    delete dialog_progress_;
    dialog_progress_ = nullptr;
}

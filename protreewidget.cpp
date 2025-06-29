#include "protreewidget.h"
#include "consts.h"
#include "protreeitem.h"
#include "removeprodialog.h"
#include "slideshowdialog.h"
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
ProTreeItem *ProTreeWidget::addProjectToTree(const QString &name,
                                             const QString &path) {
    // 拼接绝对路径（完整项目路径 = 路径 + 项目名）
    const QString file_path = QDir(path).absoluteFilePath(name);

    // 避免重复添加
    if (paths_.contains(file_path))
        return nullptr;

    // 如果目录不存在，尝试创建（确保物理路径存在）
    QDir pro_dir(file_path);
    if (!pro_dir.exists() && !pro_dir.mkpath("."))
        return nullptr;

    // 加到 set 中，防止重复
    paths_.insert(file_path);

    // 创建根节点（以此树控件为父）
    auto *item =
        new ProTreeItem(this, name, file_path, AppConsts::TreeItemType::Project);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icons/dir.png"));
    item->setData(0, Qt::ToolTipRole, file_path);

    return item;
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

    // 连接四个菜单项的槽函数
    connect(action_import_, &QAction::triggered, this,
            &ProTreeWidget::onImportProject);
    connect(action_set_start_, &QAction::triggered, this,
            &ProTreeWidget::onSetActiveProject);
    connect(action_close_pro_, &QAction::triggered, this,
            &ProTreeWidget::onCloseProject);
    connect(action_slide_show_, &QAction::triggered, this,
            &ProTreeWidget::onStartSlideshow);

    // 双击
    connect(this, &QTreeWidget::itemDoubleClicked, this,
            &ProTreeWidget::onItemDoubleClicked);
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

void ProTreeWidget::onSetActiveProject() {
    if (!right_clicked_item_)
        return;

    // 先把之前的激活项字体还原
    if (active_item_ && active_item_ != right_clicked_item_) {
        QFont font = active_item_->font(0);
        font.setBold(false);
        active_item_->setFont(0, font);
    }

    // 设置新的激活项字体加粗
    active_item_ = right_clicked_item_;
    QFont font = active_item_->font(0);
    font.setBold(true);
    active_item_->setFont(0, font);
}

void ProTreeWidget::onCloseProject() {
    // 检查 right_clicked_item_ 合法性（避免空指针崩溃）
    if (!right_clicked_item_)
        return;

    // 获取目标路径并从集合中移除
    const QString delete_path = right_clicked_item_->getFilePath();
    paths_.remove(delete_path);

    // 如果是创建的项目，就弹出自定义的对话框，提供删除本地文件的可选功能
    if (right_clicked_item_->getSource() == AppConsts::ProjectSource::Created) {
        RemoveProDialog remove_pro_dialog(this);
        if (remove_pro_dialog.exec() != QDialog::Accepted)
            return;
        // 删除本地文件（如果勾选）
        if (remove_pro_dialog.shouldDeleteLocalFile())
            QDir(delete_path).removeRecursively();
    }

    // 处理 UI：清除 active 项、删除树节点
    const int index = this->indexOfTopLevelItem(right_clicked_item_);
    if (right_clicked_item_ == active_item_)
        active_item_ = nullptr;

    // 如果当前项目有图片正在展示，且这个图片所在的项目顶层节点就是右键关闭项目的节点，则取消展示
    if (selected_item_ && right_clicked_item_ == selected_item_->getRoot()) {
        selected_item_ = nullptr;
        emit clearImageRequested();
    }

    delete this->takeTopLevelItem(index);
    right_clicked_item_ = nullptr;
}

void ProTreeWidget::onStartSlideshow() {
    if (!right_clicked_item_)
        return;
    auto *first_item = right_clicked_item_->getFirstPicItem();
    if (!first_item)
        return;
    auto *last_item = right_clicked_item_->getLastPicItem();
    if (!last_item)
        return;

    slideshow_dialog_ =
        std::make_shared<SlideshowDialog>(first_item, last_item, this);
    slideshow_dialog_->setModal(true);
    // slideshow_dialog_->showMaximized();
    slideshow_dialog_->show();
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

    connect(thread, &ProTreeThread::totalFileCountCalculated, this,
            &ProTreeWidget::onTotalFileCountCalculated);
}

void ProTreeWidget::openProject(const QString &dir_path) {
    QFileInfo info(dir_path);
    QString name = info.fileName();
    QString parent_path = info.absolutePath();

    // 添加顶层项目节点
    ProTreeItem *project_item = addProjectToTree(name, parent_path);
    if (!project_item)
        return;
    // 标记一下这个项目是导入的，后续关闭项目时，不能提供删除本地文件的功能
    project_item->setSource(AppConsts::ProjectSource::Imported);

    // 构造线程参数
    ProTreeThreadParams params{.src_path = dir_path,
                               .dest_path = dir_path,
                               .parent_item = project_item,
                               .tree_widget = this,
                               .root = project_item,
                               .operation = AppConsts::ProjectOperation::Import};

    thread_create_pro_ = std::make_shared<ProTreeThread>(params);
    connectThreadSignals();
    thread_create_pro_->start();
    showProgressDialog();
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

void ProTreeWidget::onTotalFileCountCalculated(int total) {
    if (dialog_progress_)
        dialog_progress_->setMaximum(total);
}

void ProTreeWidget::onItemDoubleClicked(QTreeWidgetItem *item, int column) {
    // 尝试转换为自定义类型 ProTreeItem
    ProTreeItem *tree_item = dynamic_cast<ProTreeItem *>(item);
    if (!tree_item)
        return;

    // 判断类型是否为图片节点
    if (tree_item->type() != static_cast<int>(AppConsts::TreeItemType::Picture))
        return;

    // 记录当前选中项，发出信号
    selected_item_ = tree_item;

    emit imagePathSelected(tree_item->getFilePath());
}

void ProTreeWidget::onPreviousClicked() {
    if (!selected_item_)
        return;
    auto *prev_item = selected_item_->getPrevItem();
    if (!prev_item)
        return;
    emit imagePathSelected(prev_item->getFilePath());
    selected_item_ = prev_item;
}

void ProTreeWidget::onNextClicked() {
    if (!selected_item_)
        return;
    auto *next_item = selected_item_->getNextItem();
    if (!next_item)
        return;
    emit imagePathSelected(next_item->getFilePath());
    selected_item_ = next_item;
}

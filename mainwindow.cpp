#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "protreedialog.h"
#include "wizard.h"
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QSplitter>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initMenus();
    initDialog();
    initSignals();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    pic_show_dialog_->resizePixmap(); // 只缩放，不重新加载图片
}

void MainWindow::initMenus() {
    initActions();
    // 创建“文件”菜单
    menu_file_ = menuBar()->addMenu(tr("文件(&F)"));
    menu_file_->addAction(action_create_pro_);
    menu_file_->addAction(action_open_pro_);

    // 创建“设置”菜单
    menu_set_ = menuBar()->addMenu(tr("设置(&S)"));
    menu_set_->addAction(action_music_);
}

void MainWindow::initActions() {
    action_create_pro_ =
        new QAction(QIcon(":/icons/createpro.png"), tr("创建项目"), this);
    action_create_pro_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    action_open_pro_ =
        new QAction(QIcon(":/icons/openpro.png"), tr("打开项目"), this);
    action_open_pro_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    action_music_ = new QAction(QIcon(":/icons/music.png"), tr("背景音乐"), this);
    action_music_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
}

// 初始化项目树对话框
void MainWindow::initDialog() {
    protree_dialog_ = new ProTreeDialog(this);
    pic_show_dialog_ = new PicShowDialog(this);

    // 添加进 splitter
    splitter_ = new QSplitter(Qt::Horizontal, this);
    splitter_->addWidget(protree_dialog_);
    splitter_->addWidget(pic_show_dialog_);

    // 设置为主窗口中心部件
    this->setCentralWidget(splitter_);
    // 设置初始尺寸比例，{3, 10} 会因为数字太小不生效
    splitter_->setSizes({300, 1000});
}

void MainWindow::initSignals() {
    connect(action_create_pro_, &QAction::triggered, this,
            &MainWindow::onCreateProject);
    connect(action_open_pro_, &QAction::triggered, this,
            &MainWindow::onOpenProject);
    connect(this, &MainWindow::openProject, protree_dialog_,
            &ProTreeDialog::onOpenProject);
    // 用于根据选中的图片路径显示图片内容，采用信号接力
    connect(protree_dialog_, &ProTreeDialog::imagePathSelected, pic_show_dialog_,
            &PicShowDialog::onImagePathSelected);
    connect(pic_show_dialog_, &PicShowDialog::previousClicked, protree_dialog_,
            &ProTreeDialog::previousClicked);
    connect(pic_show_dialog_, &PicShowDialog::nextClicked, protree_dialog_,
            &ProTreeDialog::nextClicked);
    connect(protree_dialog_, &ProTreeDialog::clearImageRequested,
            pic_show_dialog_, &PicShowDialog::onClearImageRequested);
    connect(action_music_, &QAction::triggered, protree_dialog_,
            &ProTreeDialog::setMusic);
}

void MainWindow::onCreateProject(bool) {
    // 创建向导窗口，用于输入项目名和路径
    Wizard *wizard = new Wizard(this);
    // 连接槽函数
    connect(wizard, &Wizard::projectSettingChanged, protree_dialog_,
            &ProTreeDialog::onProjectSettingChanged);
    wizard->exec();
}

void MainWindow::onOpenProject(bool) {
    QString dir_path = QFileDialog::getExistingDirectory(this, tr("选择项目目录"),
                                                         QDir::currentPath());
    if (dir_path.isEmpty())
        return;
    emit openProject(dir_path);
}

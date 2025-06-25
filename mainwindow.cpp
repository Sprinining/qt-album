#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "protreedialog.h"
#include "wizard.h"
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initMenus();
    initDialog();
    initSignals();
}

MainWindow::~MainWindow() { delete ui; }

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
    ui->verticalLayoutPro->addWidget(protree_dialog_);

    pic_show_dialog_ = new PicShowDialog(this);
    ui->verticalLayoutPic->addWidget(pic_show_dialog_);
}

void MainWindow::initSignals() {
    connect(action_create_pro_, &QAction::triggered, this,
            &MainWindow::onCreateProject);
    connect(action_open_pro_, &QAction::triggered, this,
            &MainWindow::onOpenProject);
    connect(this, &MainWindow::openProject, protree_dialog_,
            &ProTreeDialog::onOpenProject);
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

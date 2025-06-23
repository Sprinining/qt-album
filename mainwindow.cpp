#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "protreedialog.h"
#include "wizard.h"
#include <QAction>
#include <QDebug>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 创建“文件”菜单
    QMenu *menu_file = menuBar()->addMenu(tr("文件(&F)"));
    // 添加“创建项目”菜单项
    QAction *action_create_pro =
        new QAction(QIcon(":/icons/createpro.png"), tr("创建项目"), this);
    action_create_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menu_file->addAction(action_create_pro);
    // 添加“打开项目”菜单项
    QAction *action_open_pro =
        new QAction(QIcon(":/icons/openpro.png"), tr("打开项目"), this);
    action_open_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menu_file->addAction(action_open_pro);

    // 创建“设置”菜单
    QMenu *menu_set = menuBar()->addMenu(tr("设置(&S)"));
    // 添加“背景音乐”菜单项
    QAction *action_music =
        new QAction(QIcon(":/icons/music.png"), tr("背景音乐"), this);
    action_music->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    menu_set->addAction(action_music);

    // 连接槽函数
    connect(action_create_pro, &QAction::triggered, this,
            &MainWindow::onCreateProject);
    connect(action_open_pro, &QAction::triggered, this, &MainWindow::onOpenProject);

    // 初始化项目树对话框
    protree_dialog_ = new ProTreeDialog(this);
    ui->verticalLayoutPro->addWidget(protree_dialog_);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onCreateProject(bool) {
    // 创建向导窗口，用于输入项目名和路径
    Wizard *wizard = new Wizard(this);
    // 连接槽函数
    connect(wizard, &Wizard::projectSettingChanged, protree_dialog_,
            &ProTreeDialog::onProjectSettingChanged);
    wizard->exec();
}

void MainWindow::onOpenProject(bool) { qDebug() << "openPro"; }

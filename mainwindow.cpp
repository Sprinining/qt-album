#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "protreedialog.h"
#include "wizard.h"
#include <QAction>
#include <QDebug>
#include <QMenu>

// MainWindow 构造函数，初始化主窗口界面和功能
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this); // 构建 UI 并绑定控件指针

    // --------------------- 创建“文件”菜单 ---------------------
    QMenu* menu_file = menuBar()->addMenu(tr("文件(&F)")); // 添加菜单栏 -> 文件

    // 创建“创建项目”菜单项，设置图标和快捷键 Ctrl+N
    QAction* act_create_pro = new QAction(QIcon(":/icons/createpro.png"), tr("创建项目"), this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menu_file->addAction(act_create_pro); // 添加到“文件”菜单

    // 创建“打开项目”菜单项，设置图标和快捷键 Ctrl+O
    QAction* act_open_pro = new QAction(QIcon(":/icons/openpro.png"), tr("打开项目"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menu_file->addAction(act_open_pro);

    // --------------------- 创建“设置”菜单 ---------------------
    QMenu* menu_set = menuBar()->addMenu(tr("设置(&S)")); // 添加菜单栏 -> 设置

    // 添加“背景音乐”菜单项，快捷键 Ctrl+M
    QAction* act_music = new QAction(QIcon(":/icons/music.png"), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    menu_set->addAction(act_music);

    // --------------------- 连接菜单项和槽函数 ---------------------
    connect(act_create_pro, &QAction::triggered, this, &MainWindow::createPro); // 创建项目
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::openPro);     // 打开项目

    // --------------------- 初始化项目树对话框 ---------------------
    protree_dialog = new ProTreeDialog(this);         // 创建项目树对话框实例
    ui->verticalLayoutPro->addWidget(protree_dialog); // 将其添加到主窗口布局中
}

MainWindow::~MainWindow() {
    delete ui;
}

// 创建项目按钮的槽函数
void MainWindow::createPro(bool) {
    qDebug() << "createPro";

    // 创建向导窗口（Wizard），用于输入项目名和路径
    Wizard* wizard = new Wizard(this);

    // 连接 Wizard 的信号 sendProSetting 到项目树接收槽函数
    connect(wizard, &Wizard::sendProSetting, protree_dialog, &ProTreeDialog::recvProSetting);

    // 弹出模态对话框（阻塞式）
    wizard->exec();

    // 断开连接
    disconnect(wizard, &Wizard::sendProSetting, protree_dialog, &ProTreeDialog::recvProSetting);
}
void MainWindow::openPro(bool) {
    qDebug() << "openPro";
}

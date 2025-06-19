#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "wizard.h"
#include <QAction>
#include <QDebug>
#include <QMenu>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 增加文件菜单
    QMenu* menu_file = menuBar()->addMenu(tr("文件(&F)"));
    QAction* act_create_pro = new QAction(QIcon(":/icons/createpro.png"), tr("创建项目"), this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menu_file->addAction(act_create_pro);
    QAction* act_open_pro = new QAction(QIcon(":/icons/openpro.png"), tr("打开项目"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menu_file->addAction(act_open_pro);

    // 增加设置菜单
    QMenu* menu_set = menuBar()->addMenu(tr("设置(&S)"));
    QAction* act_music = new QAction(QIcon(":/icons/music.png"), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    menu_set->addAction(act_music);

    // 连接槽函数
    connect(act_create_pro, &QAction::triggered, this, &MainWindow::createPro);
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::openPro);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createPro(bool) {
    qDebug() << "createPro";
    Wizard* wizard = new Wizard(this);
    connect(wizard, &Wizard::sendProSetting, this, &MainWindow::recvProSetting);
    wizard->exec();
}

void MainWindow::openPro(bool) {
    qDebug() << "openPro";
}

void MainWindow::recvProSetting(const QString& name, const QString& path) {
    qDebug() << "recvProSetting: " << name << " " << path << Qt::endl;
}

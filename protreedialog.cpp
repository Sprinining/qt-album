#include "protreedialog.h"
#include "ui_protreedialog.h"

ProTreeDialog::ProTreeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ProTreeDialog) {
    ui->setupUi(this);
    ui->treeWidget->setHeaderLabels(QStringList() << "项目名称");
    // 信号接力
    connect(ui->treeWidget, &ProTreeWidget::imagePathSelected, this,
            &ProTreeDialog::imagePathSelected);
    connect(this, &ProTreeDialog::previousClicked, ui->treeWidget,
            &ProTreeWidget::onPreviousClicked);
    connect(this, &ProTreeDialog::nextClicked, ui->treeWidget,
            &ProTreeWidget::onNextClicked);
    connect(ui->treeWidget, &ProTreeWidget::clearImageRequested, this,
            &ProTreeDialog::clearImageRequested);
    connect(this, &ProTreeDialog::setMusic, ui->treeWidget,
            &ProTreeWidget::onSetMusic);
}

ProTreeDialog::~ProTreeDialog() { delete ui; }

void ProTreeDialog::onProjectSettingChanged(const QString &name,
                                            const QString &dir_path) {
    // 根据路径和问价夹名称，导入项目树
    ui->treeWidget->addProjectToTree(name, dir_path);
}

void ProTreeDialog::onOpenProject(const QString &dir_path) {
    ui->treeWidget->openProject(dir_path);
}

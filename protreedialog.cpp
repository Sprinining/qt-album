#include "protreedialog.h"
#include "ui_protreedialog.h"

ProTreeDialog::ProTreeDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ProTreeDialog) {
    ui->setupUi(this);
    ui->treeWidget->setHeaderLabels(QStringList() << "项目名称" << "路径");
}

ProTreeDialog::~ProTreeDialog() {
    delete ui;
}

void ProTreeDialog::recvProSetting(const QString& name, const QString& path) {
    ui->treeWidget->addProToTree(name, path);
}

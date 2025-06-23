#include "protreedialog.h"
#include "ui_protreedialog.h"

ProTreeDialog::ProTreeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ProTreeDialog) {
    ui->setupUi(this);
    ui->treeWidget->setHeaderLabels(QStringList() << "项目名称");
}

ProTreeDialog::~ProTreeDialog() { delete ui; }

void ProTreeDialog::onProjectSettingChanged(const QString &name,
                                            const QString &dir_path) {
    // 根据路径和问价夹名称，导入项目树
    ui->treeWidget->addProjectToTree(name, dir_path);
}

#include "wizard.h"
#include "ui_wizard.h"
#include <QDialog>

Wizard::Wizard(QWidget *parent) : QWizard(parent), ui(new Ui::Wizard) {
    ui->setupUi(this);
    setWindowTitle(tr("创建项目"));
}

Wizard::~Wizard() { delete ui; }

void Wizard::done(int result) {
    // 如果用户取消，则直接调用基类 done 并返回
    if (result == QDialog::Rejected) {
        QWizard::done(result);
        return;
    }

    // 向导成功完成时，获取项目名称和路径
    QString name, path;
    ui->wizardPagePorSet->getProjectSetting(name, path);

    // 发送信号给项目树对话框，传递项目的设置
    emit projectSettingChanged(name, path);

    // 调用基类 done() 以完成向导关闭操作
    QWizard::done(result);
}

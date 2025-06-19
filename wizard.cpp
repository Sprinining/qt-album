#include "wizard.h"
#include "ui_wizard.h"

Wizard::Wizard(QWidget* parent) : QWizard(parent), ui(new Ui::Wizard) {
    ui->setupUi(this);
    setWindowTitle(tr("创建项目"));
}

Wizard::~Wizard() {
    delete ui;
}

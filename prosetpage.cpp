#include "prosetpage.h"
#include "ui_prosetpage.h"

ProSetPage::ProSetPage(QWidget* parent) : QWizardPage(parent), ui(new Ui::ProSetPage) {
    ui->setupUi(this);
}

ProSetPage::~ProSetPage() {
    delete ui;
}

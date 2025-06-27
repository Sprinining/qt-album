#include "slideshowdialog.h"
#include "protreeitem.h"
#include "ui_slideshowdialog.h"

SlideshowDialog::SlideshowDialog(const ProTreeItem *first_item,
                                 const ProTreeItem *last_item, QWidget *parent)
    : QDialog(parent), ui(new Ui::SlideshowDialog()), first_item_(first_item),
    last_item_(last_item) {
    ui->setupUi(this);
}

SlideshowDialog::~SlideshowDialog() { delete ui; }

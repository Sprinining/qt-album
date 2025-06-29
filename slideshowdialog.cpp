#include "slideshowdialog.h"
#include "protreeitem.h"
#include "ui_slideshowdialog.h"

SlideshowDialog::SlideshowDialog(const ProTreeItem *first_item,
                                 const ProTreeItem *last_item, QWidget *parent)
    : QDialog(parent), ui(new Ui::SlideshowDialog()), first_item_(first_item),
    last_item_(last_item) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    initButtonIcons();
    ui->widgetAnimation->setPixmap(first_item_);
    ui->widgetAnimation->startAnimation();
}

SlideshowDialog::~SlideshowDialog() { delete ui; }

void SlideshowDialog::initButtonIcons() {
    ui->pushButtonPrev->setIcons(":/icons/previous.png",
                                 ":/icons/previous_hover.png",
                                 ":/icons/previous_press.png");
    ui->pushButtonNext->setIcons(":/icons/next.png", ":/icons/next_hover.png",
                                 ":/icons/next_press.png");
    ui->pushButtonStop->setIcons(":/icons/closeshow.png",
                                 ":/icons/closeshow_hover.png",
                                 ":/icons/closeshow_press.png");
    ui->pushButtonPlay->setIcons(":/icons/play.png", ":/icons/play_hover.png",
                                 ":/icons/play_press.png");
    ui->pushButtonPlay->setToggleIcons(":/icons/pause.png",
                                       ":/icons/pause_hover.png",
                                       ":/icons/pause_press.png");
}

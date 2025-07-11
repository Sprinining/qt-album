#include "slideshowdialog.h"
#include "protreeitem.h"
#include "ui_slideshowdialog.h"

SlideshowDialog::SlideshowDialog(const ProTreeItem *first_item,
                                 const ProTreeItem *last_item, QWidget *parent)
    : QDialog(parent), ui(new Ui::SlideshowDialog()), first_item_(first_item),
    last_item_(last_item) {
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    initButtonIcons();
    initSignals();
    ui->widgetAnimation->setPixmap(first_item_);
}

SlideshowDialog::~SlideshowDialog() { delete ui; }

void SlideshowDialog::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    ui->widgetAnimation->startAnimation();
}

void SlideshowDialog::closeEvent(QCloseEvent *event) {
    ui->widgetAnimation->stopAnimation(); // 安全释放动画资源
    emit stopMusic();                     // 发出音乐停止信号
    QDialog::closeEvent(event);           // 保留默认行为
}

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

void SlideshowDialog::initSignals() {
    connect(ui->pushButtonPlay, &QPushButton::clicked, ui->widgetAnimation,
            &AnimationWidget::onPlayOrStop);
    connect(ui->pushButtonStop, &QPushButton::clicked, this,
            &SlideshowDialog::close);
    connect(ui->pushButtonPrev, &QPushButton::clicked, this,
            &SlideshowDialog::onPrevButtonClicked);
    connect(ui->pushButtonNext, &QPushButton::clicked, this,
            &SlideshowDialog::onNextButtonClicked);
    connect(ui->widgetAnimation, &AnimationWidget::updatePreviewList,
            ui->previewListWidget, &PreviewListWidget::onUpdatePreviewList);
    connect(ui->widgetAnimation, &AnimationWidget::previewListItemSelected,
            ui->previewListWidget, &PreviewListWidget::onPreviewListItemSelected);
    connect(ui->previewListWidget, &PreviewListWidget::showSelectedItem,
            ui->widgetAnimation, &AnimationWidget::onShowSelectedItem);
    connect(ui->widgetAnimation, &AnimationWidget::animationStarted,
            ui->pushButtonPlay, &ToggleImageButton::onAnimationStarted);
    connect(ui->widgetAnimation, &AnimationWidget::animationStopped,
            ui->pushButtonPlay, &ToggleImageButton::onAnimationStopped);
    // 信号接力
    connect(ui->widgetAnimation, &AnimationWidget::startMusic, this,
            &SlideshowDialog::startMusic);
    connect(ui->widgetAnimation, &AnimationWidget::pauseMusic, this,
            &SlideshowDialog::pauseMusic);
    connect(ui->widgetAnimation, &AnimationWidget::resumeMusic, this,
            &SlideshowDialog::resumeMusic);
    connect(ui->widgetAnimation, &AnimationWidget::stopMusic, this,
            &SlideshowDialog::stopMusic);
}

void SlideshowDialog::onPrevButtonClicked() {
    ui->widgetAnimation->slidePrev();
}

void SlideshowDialog::onNextButtonClicked() {
    ui->widgetAnimation->slideNext();
}

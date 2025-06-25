#include "picshowdialog.h"
#include "ui_picshowdialog.h"

PicShowDialog::PicShowDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::PicShowDialog) {
    ui->setupUi(this);

    // 设置“上一张”按钮的三种状态图标（正常、悬停、按下）
    ui->pushButtonPrev->setIcons(":/icons/previous.png",
                                 ":/icons/previous_hover.png",
                                 ":/icons/previous_press.png");
    // 设置“下一张”按钮的三种状态图标
    ui->pushButtonNext->setIcons(":/icons/next.png", ":/icons/next_hover.png",
                                 ":/icons/next_press.png");

    // 初始化“上一张”按钮的透明度效果和动画
    setupButtonFadeEffect(ui->pushButtonPrev, effect_prev_, anim_prev_);
    // 初始化“下一张”按钮的透明度效果和动画
    setupButtonFadeEffect(ui->pushButtonNext, effect_next_, anim_next_);
}

PicShowDialog::~PicShowDialog() { delete ui; }

// 为指定按钮设置透明度效果和淡入淡出动画
void PicShowDialog::setupButtonFadeEffect(QWidget *button,
                                          QGraphicsOpacityEffect *&effect,
                                          QPropertyAnimation *&anim) {
    // 创建透明度效果，并设为完全透明
    effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(0);             // 初始状态不可见
    button->setGraphicsEffect(effect); // 应用到按钮上

    // 创建动画：操作的是透明度属性 "opacity"
    anim = new QPropertyAnimation(effect, "opacity", this);
    anim->setEasingCurve(QEasingCurve::InOutQuad); // 使用缓入缓出动画曲线
    anim->setDuration(500);                        // 动画持续时间 500 毫秒
}

// 事件过滤函数：监听鼠标进入/离开窗口，控制按钮显示隐藏
bool PicShowDialog::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::Enter:
        animateButtons(true); // 鼠标进入窗口，显示按钮（淡入）
        break;
    case QEvent::Leave:
        animateButtons(false); // 鼠标离开窗口，隐藏按钮（淡出）
        break;
    default:
        break;
    }
    return QDialog::event(event); // 交给基类继续处理其他事件
}

// 控制按钮动画显示或隐藏
void PicShowDialog::animateButtons(bool visible) {
    // 如果当前状态与目标状态一致，跳过处理，避免重复执行动画
    if (visible == btn_visible_)
        return;

    // 计算动画起始值和结束值
    qreal startValue = visible ? 0.0 : 1.0; // 淡入时从 0.0 开始
    qreal endValue = visible ? 1.0 : 0.0;   // 淡出时到 0.0 结束

    // 播放“上一张”按钮的透明度动画
    anim_prev_->stop(); // 先停止可能正在运行的动画
    anim_prev_->setStartValue(startValue);
    anim_prev_->setEndValue(endValue);
    anim_prev_->start(); // 启动动画

    // 播放“下一张”按钮的透明度动画
    anim_next_->stop();
    anim_next_->setStartValue(startValue);
    anim_next_->setEndValue(endValue);
    anim_next_->start();

    // 更新状态标志
    btn_visible_ = visible;
}

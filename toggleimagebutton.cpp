#include "toggleimagebutton.h"
#include <QEvent>
#include <QPainter>
#include <QPainterPath>

ToggleImageButton::ToggleImageButton(QWidget *parent) : QPushButton(parent) {
    // 去除按钮默认边框和背景
    setFlat(true);
    // 禁止聚焦虚线框显示
    setFocusPolicy(Qt::NoFocus);
    // 设置背景透明，边框无效
    setStyleSheet("background: transparent; border: none;");
}

ToggleImageButton::~ToggleImageButton() {}

void ToggleImageButton::setIcons(const QString &normal, const QString &hover,
                                 const QString &pressed) {
    normal_path_ = normal;
    hover_path_ = hover;
    pressed_path_ = pressed;

    // 加载三张图片
    pixmap_normal_.load(normal_path_);
    pixmap_hover_.load(hover_path_);
    pixmap_pressed_.load(pressed_path_);

    // 如果正常图加载成功，调整按钮大小匹配图标大小
    if (!pixmap_normal_.isNull()) {
        this->resize(pixmap_normal_.size());
    } else {
        qWarning() << "Failed to load normal icon:" << normal_path_;
    }
}

void ToggleImageButton::setToggleIcons(const QString &normal,
                                       const QString &hover,
                                       const QString &pressed) {

    toggled_normal_path_ = normal;
    toggled_hover_path_ = hover;
    toggled_pressed_path_ = pressed;

    // 加载三张图片
    pixmap_toggled_normal_.load(toggled_normal_path_);
    pixmap_toggled_hover_.load(toggled_hover_path_);
    pixmap_toggled_pressed_.load(toggled_pressed_path_);
}

// 事件处理函数，监听鼠标状态变化切换按钮状态，并触发重绘
bool ToggleImageButton::event(QEvent *e) {
    switch (e->type()) {
    case QEvent::Enter: // 鼠标进入
        current_state_ = ButtonState::Hover;
        update(); // 触发 paintEvent 重绘
        break;
    case QEvent::Leave: // 鼠标离开
        current_state_ = ButtonState::Normal;
        update();
        break;
    case QEvent::MouseButtonPress: // 鼠标按下
        current_state_ = ButtonState::Pressed;
        update();
        break;
    case QEvent::MouseButtonRelease: // 鼠标释放
        // 切换触发态
        is_toggled_ = !is_toggled_;
        current_state_ = ButtonState::Hover;
        update();
        break;
    default:
        break;
    }
    return QPushButton::event(e); // 继续传递事件给基类处理
}

void ToggleImageButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // 启用抗锯齿，令绘制边缘更平滑，避免锯齿状边缘
    painter.setRenderHint(QPainter::Antialiasing);

    // 获取当前状态对应的图标图片
    QPixmap pix = currentPixmap();
    // 如果图片无效（未加载成功），则不绘制，直接返回
    if (pix.isNull())
        return;

    // 将图标绘制到按钮的整个矩形区域（this->rect()）
    // 图片会根据按钮大小自动拉伸或缩放，铺满整个按钮
    painter.drawPixmap(this->rect(), pix);
}

// 根据当前状态返回对应的图标 QPixmap
QPixmap ToggleImageButton::currentPixmap() const {
    if (is_toggled_) {
        switch (current_state_) {
        case ButtonState::Hover:
            return pixmap_toggled_hover_;
        case ButtonState::Pressed:
            return pixmap_toggled_pressed_;
        case ButtonState::Normal:
        default:
            return pixmap_toggled_normal_;
        }
    } else {
        switch (current_state_) {
        case ButtonState::Hover:
            return pixmap_hover_;
        case ButtonState::Pressed:
            return pixmap_pressed_;
        case ButtonState::Normal:
        default:
            return pixmap_normal_;
        }
    }
}

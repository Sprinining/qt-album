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

// 自定义绘制函数，重写 QPushButton 的绘制行为
// 目标：绘制一个圆形裁剪的图标，铺满整个按钮区域
void ToggleImageButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // 启用抗锯齿，使绘制边缘更加平滑，避免锯齿状边缘
    painter.setRenderHint(QPainter::Antialiasing);

    // 获取当前按钮状态对应的图标 QPixmap
    QPixmap pix = currentPixmap();
    if (pix.isNull())
        return;

    // 创建一个绘图路径，用于限定绘制区域形状
    QPainterPath path;

    // 向路径中添加一个椭圆，椭圆区域与按钮当前矩形区域（this->rect()）相同
    // 因为按钮是矩形的，添加的椭圆就是“内切圆”，实现圆形裁剪
    path.addEllipse(this->rect());

    // 设置 QPainter 的裁剪区域为上面定义的圆形路径
    // 后续绘图只能在圆形区域内显示，超出圆形部分的内容将被裁剪掉（不显示）
    painter.setClipPath(path);

    // 将图标绘制到按钮的整个矩形区域（this->rect()），
    // 图标会根据按钮大小拉伸或缩放，铺满整个按钮
    // 为了避免变形，传入的图片宽高相等（正方形）
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

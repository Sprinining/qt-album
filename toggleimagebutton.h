#ifndef TOGGLEIMAGEBUTTON_H
#define TOGGLEIMAGEBUTTON_H

#include <QPushButton>

class ToggleImageButton : public QPushButton {
public:
    explicit ToggleImageButton(QWidget *parent = nullptr);
    ~ToggleImageButton();

    void setIcons(const QString &normal, const QString &hover,
                  const QString &pressed);
    void setToggleIcons(const QString &normal, const QString &hover,
                        const QString &pressed);

protected:
    // 重写事件函数，捕获鼠标进入/离开/按下/释放事件，用于切换状态
    bool event(QEvent *e) override;
    // 重写绘制事件，自定义绘制圆形图标
    void paintEvent(QPaintEvent *event) override;

private:
    // 当前按钮状态枚举
    enum class ButtonState { Normal, Hover, Pressed };
    // 记录当前状态
    ButtonState current_state_ = ButtonState::Normal;

    // 当前是否处于 Toggle 状态组（切换图）
    bool is_toggled_ = false;

    // 默认图标路径 & 缓存
    QString normal_path_, hover_path_, pressed_path_;
    QPixmap pixmap_normal_, pixmap_hover_, pixmap_pressed_;
    // Toggle 图标路径 & 缓存
    QString toggled_normal_path_, toggled_hover_path_, toggled_pressed_path_;
    QPixmap pixmap_toggled_normal_, pixmap_toggled_hover_,
        pixmap_toggled_pressed_;

    // 根据当前状态返回对应图标
    QPixmap currentPixmap() const;
};

#endif // TOGGLEIMAGEBUTTON_H

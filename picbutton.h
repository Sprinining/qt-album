#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPixmap>
#include <QPushButton>

// PicButton：支持鼠标悬停、按下状态，圆形显示图标的自定义按钮
class PicButton : public QPushButton {
    Q_OBJECT
public:
    explicit PicButton(QWidget *parent = nullptr);
    ~PicButton();

    // 设置三种状态图标的文件路径
    void setIcons(const QString &normal, const QString &hover,
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

    // 三种状态的图标路径（可选备用）
    QString normal_, hover_, pressed_;
    // 三种状态图标的图片缓存
    QPixmap pixmap_normal_, pixmap_hover_, pixmap_pressed_;

    // 根据当前状态返回对应图标
    QPixmap currentPixmap() const;
};

#endif // PICBUTTON_H

#ifndef PICSHOWDIALOG_H
#define PICSHOWDIALOG_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace Ui {
class PicShowDialog;
}

class PicShowDialog : public QDialog {
    Q_OBJECT

public:
    explicit PicShowDialog(QWidget *parent = nullptr);
    ~PicShowDialog();
    void resizePixmap();

protected:
    // 重写事件处理函数，用于处理鼠标进入/离开事件，触发按钮显隐动画
    bool event(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::PicShowDialog *ui;

    // 要显示的图片的路径
    QString selected_path_;
    QPixmap pix_map_;
    // 用于控制两个按钮的透明度效果（淡入淡出）
    QGraphicsOpacityEffect *effect_prev_ = nullptr;
    QGraphicsOpacityEffect *effect_next_ = nullptr;
    // 用于控制两个按钮透明度动画的属性动画对象
    QPropertyAnimation *anim_prev_ = nullptr;
    QPropertyAnimation *anim_next_ = nullptr;
    // 当前按钮是否可见的标志，避免重复动画触发
    bool btn_visible_ = false;

    // 初始化按钮的透明度效果和对应动画
    // 参数 button 是目标按钮，effect 是透明度效果，anim 是动画引用
    void setupButtonFadeEffect(QWidget *button, QGraphicsOpacityEffect *&effect,
                               QPropertyAnimation *&anim);
    // 根据 visible 控制按钮显示或隐藏，并执行淡入淡出动画
    void animateButtons(bool visible);
    void loadImage(const QString &file_path);
    void initSignals();

public slots:
    // 接收要展示的图片的路径
    void onImagePathSelected(const QString &file_path);
    void onClearImageRequested();
signals:
    void previousClicked();
    void nextClicked();
};

#endif // PICSHOWDIALOG_H

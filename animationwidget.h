#ifndef ANIMATIONWIDGET_H
#define ANIMATIONWIDGET_H

#include "protreeitem.h"
#include <QMap>
#include <QPixmap>
#include <QTimer>
#include <QWidget>

class AnimationWidget : public QWidget {
    Q_OBJECT

public:
    explicit AnimationWidget(QWidget *parent = nullptr);
    ~AnimationWidget();

    // 设置当前图片项，加载当前图片及下一张图片
    void setPixmap(const ProTreeItem *item);
    // 开始动画播放
    void startAnimation();
    // 停止动画播放
    void stopAnimation();
    void slidePrev();
    void slideNext();

protected:
    // 重写绘图事件，实现渐变绘制
    void paintEvent(QPaintEvent *event) override;

private:
    // 动画定时器，控制渐变更新
    QTimer *animation_timer_ = nullptr;
    // 停顿定时器，用于渐变后停留0.5秒
    QTimer *pause_timer_ = nullptr;

    // 融合因子，范围0~1
    float blend_factor_ = 0.0f;
    // 标记动画是否正在播放
    bool is_animating_ = false;
    // 标记当前是否处于停顿状态
    bool is_pause_ = false;
    // 是否动画准备阶段（初始停顿）
    bool is_initial_pause_ = true;
    // 音乐是否已经开始过
    bool is_music_started_ = false;

    // 当前显示图片
    QPixmap from_pixmap_;
    // 下一张图片，用于渐变显示
    QPixmap to_pixmap_;

    // 当前图片项指针
    const ProTreeItem *current_item_ = nullptr;
    // 缓存路径和对应图片项，避免重复加载
    QMap<QString, const ProTreeItem *> item_cache_;

    void slideTo(const ProTreeItem *target);

private slots:
    // 动画定时器超时槽，更新融合因子，触发重绘
    void onTimeout();
    // 停顿定时器超时槽，切换到下一张图片并恢复动画
    void onPauseTimeout();
public slots:
    // 展示出预览列表选中的图片
    void onShowSelectedItem(const QString &path);
    void onPlayOrStop();

signals:
    // 更新预览列表
    void updatePreviewList(const ProTreeItem *);
    // 控制下方预览列表选中 item
    void previewListItemSelected(const ProTreeItem *);
    // 动画开始或者停止的信号，用于控制开始按钮的显示图标
    void animationStarted();
    void animationStopped();
    // 信号接力
    void startMusic();
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
};

#endif // ANIMATIONWIDGET_H

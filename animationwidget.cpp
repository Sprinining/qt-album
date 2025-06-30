#include "animationwidget.h"
#include <QPainter>

AnimationWidget::AnimationWidget(QWidget *parent)
    : QWidget(parent), animation_timer_(new QTimer(this)),
      pause_timer_(new QTimer(this)), is_pause_(false) {
    connect(animation_timer_, &QTimer::timeout, this,
            &AnimationWidget::onTimeout);
    connect(pause_timer_, &QTimer::timeout, this,
            &AnimationWidget::onPauseTimeout);

    // 停顿定时器为单次触发
    pause_timer_->setSingleShot(true);
}

AnimationWidget::~AnimationWidget() { emit stopMusic(); }

// 设置当前图片项，加载当前图片和下一张图片，用于动画渐变
void AnimationWidget::setPixmap(const ProTreeItem *item) {
    if (item == nullptr)
        return;

    // 加载当前图片路径
    const QString &path = item->getFilePath();
    if (!from_pixmap_.load(path))
        return;
    current_item_ = item;
    // 缓存当前图片项，防止重复加载
    if (!item_cache_.contains(path)) {
        item_cache_.insert(path, item);
        // 发送信号，更新预览列表
        emit updatePreviewList(item);
    }
    emit previewListItemSelected(item);

    // 预加载下一张图片，准备动画渐变
    const ProTreeItem *next_item = item->getNextItem();
    if (next_item == nullptr) {
        to_pixmap_ = QPixmap(); // 清空下一张图，防止残留
        return;
    }
    const QString &next_path = next_item->getFilePath();
    to_pixmap_.load(next_path);
    // 缓存下一张图片项
    if (!item_cache_.contains(next_path)) {
        item_cache_.insert(next_path, next_item);
        emit updatePreviewList(next_item);
    }
}

// 开始动画，重置融合因子，启动动画定时器
void AnimationWidget::startAnimation() {
    if (is_animating_)
        return;

    blend_factor_ = 0.0f;
    is_animating_ = true;
    is_pause_ = true;         // 先进入停顿状态
    is_initial_pause_ = true; // 表示是初始停顿

    pause_timer_->start(700); // 初始停顿 700ms 后开始动画

    emit animationStarted(); // 发射动画开始信号
    if (!is_music_started_) {
        emit startMusic();
        is_music_started_ = true;
    } else {
        emit resumeMusic();
    }
}

// 停止动画，停止所有定时器，重置状态，并触发重绘刷新
void AnimationWidget::stopAnimation() {
    animation_timer_->stop();
    pause_timer_->stop();
    blend_factor_ = 0.0f;
    is_animating_ = false;
    is_pause_ = false;
    update();
    emit animationStopped(); // 发射动画停止信号
    emit pauseMusic();
}

// 切换至指定图片项（用于上一张或下一张切换）
void AnimationWidget::slideTo(const ProTreeItem *target) {
    // 停止当前动画（防止重叠播放）
    stopAnimation();

    // 判空，避免非法访问
    if (!target)
        return;

    // 设置新的图片项，准备动画渲染
    setPixmap(target);

    // 请求重绘，立即更新界面显示
    update();
}

// 切换到上一张图片
void AnimationWidget::slidePrev() {
    // 如果当前图片项存在，则尝试切换到其前一项
    if (current_item_)
        slideTo(current_item_->getPrevItem());
}

// 切换到下一张图片
void AnimationWidget::slideNext() {
    // 如果当前图片项存在，则尝试切换到其后一项
    if (current_item_)
        slideTo(current_item_->getNextItem());
}

// 重写绘图事件，实现当前图片和平滑过渡到下一张图片的透明度渐变叠加绘制
void AnimationWidget::paintEvent(QPaintEvent *) {
    // 第1步：创建绘图对象，准备在控件上绘制
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 第2步：获取控件宽高
    int widget_width = width();
    int widget_height = height();

    // ==== 处理当前图片 from_pixmap_ ====

    // 第3-from：如果当前图片为空，直接返回
    if (from_pixmap_.isNull())
        return;

    // 第4-from：缩放当前图片，保持宽高比
    QPixmap scaled_current_pixmap =
        from_pixmap_.scaled(widget_width, widget_height, Qt::KeepAspectRatio);

    // 第5-from：计算当前图片透明度，blend_factor_ 从 0 到 1，透明度从 255 变到 0
    int alpha_current = static_cast<int>(255 * (1.0f - blend_factor_));

    // 第6-from：创建透明画布
    QPixmap current_pixmap_with_alpha(scaled_current_pixmap.size());
    current_pixmap_with_alpha.fill(Qt::transparent);

    // 第7-from：绘制当前图片并加透明度遮罩
    {
        QPainter current_painter(&current_pixmap_with_alpha);
        current_painter.setCompositionMode(QPainter::CompositionMode_Source);
        current_painter.drawPixmap(0, 0, scaled_current_pixmap);
        current_painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        current_painter.fillRect(current_pixmap_with_alpha.rect(),
                                 QColor(0, 0, 0, alpha_current));
        current_painter.end();
    }

    // 第8-from：计算绘制位置，居中显示
    int draw_x = (widget_width - scaled_current_pixmap.width()) / 2;
    int draw_y = (widget_height - scaled_current_pixmap.height()) / 2;

    // 第9-from：绘制带透明度的当前图片
    painter.drawPixmap(draw_x, draw_y, current_pixmap_with_alpha);

    // ==== 处理下一张图片 to_pixmap_ ====

    // 第3-to：如果下一张图片为空，退出
    if (to_pixmap_.isNull())
        return;

    // 第4-to：缩放下一张图片，保持宽高比
    QPixmap scaled_next_pixmap =
        to_pixmap_.scaled(widget_width, widget_height, Qt::KeepAspectRatio);

    // 第5-to：计算下一张图片透明度，blend_factor_ 从 0 到 1，透明度从 0 变到 255
    int alpha_next = static_cast<int>(255 * blend_factor_);

    // 第6-to：创建透明画布
    QPixmap next_pixmap_with_alpha(scaled_next_pixmap.size());
    next_pixmap_with_alpha.fill(Qt::transparent);

    // 第7-to：绘制下一张图片并加透明度遮罩
    {
        QPainter next_painter(&next_pixmap_with_alpha);
        next_painter.setCompositionMode(QPainter::CompositionMode_Source);
        next_painter.drawPixmap(0, 0, scaled_next_pixmap);
        next_painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        next_painter.fillRect(next_pixmap_with_alpha.rect(),
                              QColor(0, 0, 0, alpha_next));
        next_painter.end();
    }

    // 第8-to：计算绘制位置，居中显示
    draw_x = (widget_width - scaled_next_pixmap.width()) / 2;
    draw_y = (widget_height - scaled_next_pixmap.height()) / 2;

    // 第9-to：绘制带透明度的下一张图片
    painter.drawPixmap(draw_x, draw_y, next_pixmap_with_alpha);
}

// 动画定时器超时槽，更新融合因子，触发重绘
void AnimationWidget::onTimeout() {
    if (current_item_ == nullptr) {
        stopAnimation();
        return;
    }

    if (is_pause_) {
        // 停顿状态不更新动画
        return;
    }

    blend_factor_ += 0.02f;

    if (blend_factor_ >= 1.0f) {
        blend_factor_ = 1.0f;
        is_pause_ = true;         // 进入停顿
        animation_timer_->stop(); // 停止动画定时器
        pause_timer_->start(700); // 700ms 停顿定时器
        update();
        return;
    }
    update();
}

// 停顿定时器超时槽，切换下一张并恢复动画
void AnimationWidget::onPauseTimeout() {
    if (is_initial_pause_) {
        // 初始停顿结束，开始动画
        is_pause_ = false;
        is_initial_pause_ = false;
        animation_timer_->start(30);
    } else {
        // 正常动画停顿结束，切换下一张
        is_pause_ = false;

        const ProTreeItem *next = current_item_->getNextItem();
        if (next == nullptr) {
            stopAnimation();
            return;
        }

        setPixmap(next);
        blend_factor_ = 0.0f;
        animation_timer_->start(30);
    }
}

void AnimationWidget::onShowSelectedItem(const QString &path) {
    // 从缓存中查找对应项
    auto cache_iter = item_cache_.find(path);
    if (cache_iter == item_cache_.end())
        return;

    const ProTreeItem *item = cache_iter.value();
    if (!item)
        return;
    // 加载当前图片，如果失败则返回
    if (!from_pixmap_.load(path))
        return;
    current_item_ = item;
    // 若未缓存该路径项，则加入缓存
    item_cache_.insert(path, item); // 若已存在，Qt 会自动忽略

    // 获取下一张图片项
    const ProTreeItem *next_item = item->getNextItem();
    if (!next_item)
        return;
    const QString &next_path = next_item->getFilePath();
    if (next_path.isEmpty() || !to_pixmap_.load(next_path))
        return;
    item_cache_.insert(next_path, next_item); // 同样自动忽略已存在项

    update(); // 触发重绘显示图片
}

void AnimationWidget::onPlayOrStop() {
    if (is_animating_) {
        stopAnimation();
    } else {
        startAnimation();
    }
}

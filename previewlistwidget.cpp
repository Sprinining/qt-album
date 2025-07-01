#include "previewlistwidget.h"

#include "previewlistitem.h"
#include <QGuiApplication>
#include <QPainter>

// 构造函数，初始化 QListWidget，设置视图模式、图标大小和间距
PreviewListWidget::PreviewListWidget(QWidget *parent) : QListWidget(parent) {
    initUI();
    connect(this, &PreviewListWidget::itemPressed, this,
            &PreviewListWidget::onItemPressed);
}

// 析构函数，清空缓存
// QListWidget 会自动管理 QListWidgetItem 的内存，无需手动删除
// item_cache_ 只是保存指针，方便快速查找，不拥有所有权
PreviewListWidget::~PreviewListWidget() { item_cache_.clear(); }

void PreviewListWidget::initUI() {
    setWrapping(false); // 禁止自动换行，只显示一行，超出内容不自动换行
    setFlow(QListView::LeftToRight); // 项目从左到右排列（横向流）
    setResizeMode(
        QListView::
        Adjust); // 调整模式：控件大小变化时自动调整项布局（比固定模式更灵活）
    setViewMode(QListWidget::IconMode); // 设置为图标模式（非默认的列表模式）
    setIconSize(QSize(
        AppConsts::UIConfig::PreviewListItemSize,
        AppConsts::UIConfig::PreviewListItemSize)); // 设置每项图标的显示尺寸
    setSpacing(5); // 设置项之间的水平/垂直间距（图标之间的间隙）
    setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff); // 禁止显示水平滚动条，防止占用空间
    setVerticalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff); // 禁止显示垂直滚动条，避免因横向滚动条占位而误触发
}

// 添加一个预览列表项，传入图片路径
void PreviewListWidget::addListItem(const QString &path) {
    QPixmap src_pixmap(path);
    if (src_pixmap.isNull()) {
        qWarning() << "Failed to load image:" << path;
        return;
    }

    // 按预设大小等比例缩放图片
    QPixmap scaled_pixmap = src_pixmap.scaled(
        AppConsts::UIConfig::PreviewListItemSize,
        AppConsts::UIConfig::PreviewListItemSize, Qt::KeepAspectRatio);

    // 创建目标 pixmap 作为背景，带透明灰色填充
    QPixmap dest_pixmap(AppConsts::UIConfig::PreviewListItemSize,
                        AppConsts::UIConfig::PreviewListItemSize);
    dest_pixmap.fill(QColor(46, 47, 48));

    // 在目标 pixmap 居中绘制缩放后的图片
    QPainter painter(&dest_pixmap);
    int scaled_width = scaled_pixmap.width();
    int scaled_height = scaled_pixmap.height();
    int dest_width = dest_pixmap.width();
    int dest_height = dest_pixmap.height();

    int x = (dest_width - scaled_width) / 2;
    int y = (dest_height - scaled_height) / 2;

    painter.drawPixmap(x, y, scaled_pixmap);
    painter.end(); // 显式结束绘制，释放资源

    ++item_count_; // 预览项计数加一

    // 创建新的预览列表项，传入索引、图标和路径，父控件为 this
    PreviewListItem *preview_list_item =
        new PreviewListItem(item_count_, QIcon(dest_pixmap), path, this);

    // 设置列表项大小提示，控制显示尺寸
    preview_list_item->setSizeHint(
        QSize(AppConsts::UIConfig::PreviewListItemSize,
              AppConsts::UIConfig::PreviewListItemSize));

    // 缓存该项，避免重复添加
    item_cache_.insert(path, preview_list_item);

    // 记录控件首次添加项时的位置
    if (item_count_ == 1)
        position_ = this->pos();
}

// 槽函数：更新预览列表，传入 ProTreeItem 指针
// 若传入项为空或已存在相同路径，则不处理
void PreviewListWidget::onUpdatePreviewList(const ProTreeItem *item) {
    if (item == nullptr)
        return;

    const QString &path = item->getFilePath();

    // 若缓存已存在该路径，则跳过
    if (item_cache_.contains(path))
        return;

    // 添加新预览项
    addListItem(path);
}

void PreviewListWidget::onPreviewListItemSelected(const ProTreeItem *item) {
    if (!item)
        return;

    const QString &path = item->getFilePath();
    auto it = item_cache_.find(path);
    if (it == item_cache_.end())
        return;

    PreviewListItem *preview_item = it.value();
    int index = preview_item->getIndex();

    if (index > AppConsts::UIConfig::PreviewListMaxLength) {
        int offset =
            (index - last_index_) * AppConsts::UIConfig::PreviewListItemSize;
        QPoint current_pos = this->pos();
        this->move(current_pos.x() - offset, current_pos.y());
    } else {
        this->move(position_);
        last_index_ = AppConsts::UIConfig::PreviewListMaxLength;
    }

    // 设置选中项
    this->setCurrentItem(preview_item);
}

void PreviewListWidget::onItemPressed(QListWidgetItem *item) {
    // 仅处理左键点击
    if (QGuiApplication::mouseButtons() != Qt::LeftButton)
        return;

    // 安全转换，确保 item 是 PreviewListItem 类型
    auto *clicked_item = dynamic_cast<PreviewListItem *>(item);
    if (!clicked_item)
        return;

    // 设置当前选中项
    setCurrentItem(clicked_item);

    // 发射选中项的路径信号
    emit showSelectedItem(clicked_item->getPath());
}

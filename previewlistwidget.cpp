#include "previewlistwidget.h"

#include "previewlistitem.h"
#include <QPainter>

// 构造函数，初始化 QListWidget，设置视图模式、图标大小和间距
PreviewListWidget::PreviewListWidget(QWidget *parent) : QListWidget(parent) {
    setViewMode(QListWidget::IconMode); // 图标视图模式
    setIconSize(QSize(AppConsts::UIConfig::PreviewListItemSize,
                      AppConsts::UIConfig::PreviewListItemSize)); // 设置图标大小
    setSpacing(5);                                                // 设置图标间距
}

// 析构函数，清空缓存
// QListWidget 会自动管理 QListWidgetItem 的内存，无需手动删除
// item_cache_ 只是保存指针，方便快速查找，不拥有所有权
PreviewListWidget::~PreviewListWidget() { item_cache_.clear(); }

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
    dest_pixmap.fill(QColor(220, 220, 220, 50));

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

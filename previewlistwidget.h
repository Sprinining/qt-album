#ifndef PREVIEWLISTWIDGET_H
#define PREVIEWLISTWIDGET_H

#include "previewlistitem.h"
#include "protreeitem.h"
#include <QListWidget>

class PreviewListWidget : public QListWidget {
    Q_OBJECT
public:
    explicit PreviewListWidget(QWidget *parent = nullptr);
    // 析构函数，item_cache_ 中指针由 Qt 自动管理，不需手动删除
    ~PreviewListWidget();

private:
    // 预览项计数，用于生成索引
    int item_count_ = 0;
    int last_index_ = AppConsts::UIConfig::PreviewListMaxLength;
    // 路径到预览项指针的缓存
    QMap<QString, PreviewListItem *> item_cache_;
    QPoint position_;

    void initUI();
    // 添加一个新预览项，传入图片路径
    void addListItem(const QString &path);

public slots:
    // 接收外部通知更新预览列表，参数为 ProTreeItem 指针
    void onUpdatePreviewList(const ProTreeItem *item);
    void onPreviewListItemSelected(const ProTreeItem *);
    // 预览项被点击
    void onItemPressed(QListWidgetItem *item);
signals:
    // 选中预览列表的图片，在幻灯片中展示
    void showSelectedItem(QString path);
};

#endif // PREVIEWLISTWIDGET_H

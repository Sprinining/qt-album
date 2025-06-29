#ifndef PREVIEWLISTITEM_H
#define PREVIEWLISTITEM_H

#include <QListWidgetItem>

class PreviewListItem : public QListWidgetItem {
public:
    explicit PreviewListItem(int index, const QIcon &icon, const QString &text,
                             QListWidget *listview = nullptr, int type = Type);
    int getIndex() const;
    QString getPath() const;

private:
    // 文件路径，默认用 text 初始化
    QString path_;
    // 原始数据中的索引
    int index_ = 0;
};

#endif // PREVIEWLISTITEM_H

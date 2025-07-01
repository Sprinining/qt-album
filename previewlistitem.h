#ifndef PREVIEWLISTITEM_H
#define PREVIEWLISTITEM_H

#include <QListWidgetItem>

class PreviewListItem : public QListWidgetItem {
public:
    explicit PreviewListItem(const QIcon &icon, const QString &text,
                             QListWidget *listview = nullptr, int type = Type);
    QString getPath() const;

private:
    // 文件路径，默认用 text 初始化
    QString path_;
};

#endif // PREVIEWLISTITEM_H

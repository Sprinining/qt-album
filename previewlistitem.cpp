#include "previewlistitem.h"

PreviewListItem::PreviewListItem(const QIcon &icon,
                                 const QString &text, QListWidget *listview,
                                 int type)
    : QListWidgetItem(icon, text, listview, type), path_(text) {}

QString PreviewListItem::getPath() const { return path_; }

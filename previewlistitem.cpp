#include "previewlistitem.h"

PreviewListItem::PreviewListItem(int index, const QIcon &icon,
                                 const QString &text, QListWidget *listview,
                                 int type)
    : QListWidgetItem(icon, text, listview, type), index_(index), path_(text) {}

QString PreviewListItem::getPath() const { return path_; }

int PreviewListItem::getIndex() const { return index_; }

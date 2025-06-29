#ifndef SLIDESHOWDIALOG_H
#define SLIDESHOWDIALOG_H

#include <QDialog>

namespace Ui {
class SlideshowDialog;
}

class ProTreeItem;

class SlideshowDialog : public QDialog {
    Q_OBJECT

public:
    explicit SlideshowDialog(const ProTreeItem *first_item = nullptr,
                             const ProTreeItem *last_item = nullptr,
                             QWidget *parent = nullptr);
    ~SlideshowDialog();

private:
    Ui::SlideshowDialog *ui;
    const ProTreeItem *first_item_ = nullptr;
    const ProTreeItem *last_item_ = nullptr;

    void initButtonIcons();
    void initSignals();
};

#endif // SLIDESHOWDIALOG_H

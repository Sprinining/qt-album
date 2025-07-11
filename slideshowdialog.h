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

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::SlideshowDialog *ui;
    const ProTreeItem *first_item_ = nullptr;
    const ProTreeItem *last_item_ = nullptr;

    void initButtonIcons();
    void initSignals();

private slots:
    void onPrevButtonClicked();
    void onNextButtonClicked();

signals:
    // 信号接力
    void startMusic();
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
};

#endif // SLIDESHOWDIALOG_H

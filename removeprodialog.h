#ifndef REMOVEPRODIALOG_H
#define REMOVEPRODIALOG_H

#include <QDialog>

namespace Ui {
class RemoveProDialog;
}

class RemoveProDialog : public QDialog {
    Q_OBJECT

public:
    explicit RemoveProDialog(QWidget *parent = nullptr);
    ~RemoveProDialog();
    // 是否要删除本地文件
    bool shouldDeleteLocalFile() const;

private:
    Ui::RemoveProDialog *ui;
};

#endif // REMOVEPRODIALOG_H

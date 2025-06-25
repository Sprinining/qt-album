#ifndef PROTREEDIALOG_H
#define PROTREEDIALOG_H

#include "protreewidget.h"
#include <QDialog>

namespace Ui {
class ProTreeDialog;
}

class ProTreeDialog : public QDialog {
    Q_OBJECT

public:
    explicit ProTreeDialog(QWidget *parent = nullptr);
    ~ProTreeDialog();

private:
    Ui::ProTreeDialog *ui;

public slots:
    // 接收项目设置信息的槽函数
    void onProjectSettingChanged(const QString &name, const QString &dir_path);
    void onOpenProject(const QString &dir_path);
signals:
    // 信号接力传递选中的图片的路径
    void imagePathSelected(QString file_path);
};

#endif // PROTREEDIALOG_H

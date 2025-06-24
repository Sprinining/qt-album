#ifndef PROTREEDIALOG_H
#define PROTREEDIALOG_H

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
};

#endif // PROTREEDIALOG_H

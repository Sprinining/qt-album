#ifndef PROTREEDIALOG_H
#define PROTREEDIALOG_H

#include <QDialog>

namespace Ui {
class ProTreeDialog;
}

// 项目树对话框类，用于展示用户创建或打开的项目结构
// 继承自 QDialog，表示这是一个对话框窗口
class ProTreeDialog : public QDialog {
    Q_OBJECT // 启用 Qt 的元对象系统，支持信号与槽机制

public : explicit ProTreeDialog(QWidget *parent = nullptr);
    ~ProTreeDialog();

private:
    Ui::ProTreeDialog *ui;

public slots:
    /**
   * @brief onProjectSettingChanged 接收项目设置信息的槽函数
   *        通常由创建向导窗口（如 Wizard）发送信号触发，用于初始化项目树显示
   * @param name 项目名称
   * @param path 项目路径
   */
    void onProjectSettingChanged(const QString &name, const QString &path);
};

#endif // PROTREEDIALOG_H

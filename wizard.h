#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>

namespace Ui {
class Wizard;
}

class Wizard : public QWizard {
    Q_OBJECT

public:
    explicit Wizard(QWidget *parent = nullptr);
    ~Wizard();

protected:
    // 重写 QWizard 的 done() 方法，处理向导完成时的逻辑
    // 参数 result 表示向导结束的结果（如 Accepted 或 Rejected）
    void done(int result) override;

private:
    Ui::Wizard *ui;

signals:
    // 自定义信号，发送项目名称和路径字符串
    // 这里参数使用传值，安全且高效（QString 隐式共享）
    void projectSettingChanged(QString name, QString dir_path);
};

#endif // WIZARD_H

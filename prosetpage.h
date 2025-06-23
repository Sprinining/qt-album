#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include "consts.h" // 必须包含，不能只前向声明
#include <QWizardPage>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage {
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();
    // 返回从向导页获得的信息
    void getProjectSetting(QString &name, QString &dir_path) const;

protected:
    // 用于确定“下一步”或“完成”按钮是否应该被启用或禁用
    bool isComplete() const override;

private:
    Ui::ProSetPage *ui;

    // 共用输入校验逻辑
    AppConsts::InputStatus validateInput() const;
    void initUI();
    void initSignals();

private slots:
    // 实时检查输入，更新提示
    void onCheckInput();
    void on_pushButtonBrowse_clicked();
};

#endif // PROSETPAGE_H

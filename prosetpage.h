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
    void getProSetting(QString &name, QString &path);

protected:
    bool isComplete() const override;

private slots:
    void checkInput(); // 实时检查输入，更新提示

    void on_pushButtonBrowse_clicked();

private:
    AppConsts::InputStatus validateInput() const; // 共用输入校验逻辑

private:
    Ui::ProSetPage *ui;
};

#endif // PROSETPAGE_H

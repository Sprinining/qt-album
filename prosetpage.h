#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage {
    Q_OBJECT

  public:
    explicit ProSetPage(QWidget* parent = nullptr);
    ~ProSetPage();
    void getProSetting(QString& name, QString& path);

  protected:
    bool isComplete() const override;

  private slots:
    void checkInput(); // 实时检查输入，更新提示

    void on_pushButtonBrowse_clicked();

  private:
    // 使用枚举类型 InputStatus 来表示输入校验的不同状态，
    // 优点包括：
    // 1. 代码语义清晰，返回值更具可读性，不用靠魔法数字或字符串判断结果。
    // 2. 方便统一管理各种输入错误状态，便于扩展和维护。
    // 3. 避免在多个函数中重复写复杂的判断逻辑，提升代码复用性和一致性。
    // 4. 使用 switch 语句处理不同状态时，代码结构更整洁，易于调试和修改。
    enum InputStatus {
        Valid,        // 输入合法，没有错误
        EmptyField,   // 输入字段为空
        PathNotExist, // 指定路径不存在
        ProjectExists // 项目路径已存在，冲突
    };
    InputStatus validateInput() const; // 共用输入校验逻辑

  private:
    Ui::ProSetPage* ui;
};

#endif // PROSETPAGE_H

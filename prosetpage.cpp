#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QDir>
#include <QLineEdit>

ProSetPage::ProSetPage(QWidget* parent) : QWizardPage(parent), ui(new Ui::ProSetPage) {
    ui->setupUi(this);
    setTitle(tr("设置项目配置"));

    // 设置提示标签字体颜色为红色
    ui->labelTips->setStyleSheet("color: red;");

    // 注册字段，带 * 表示必须填写
    registerField("proName*", ui->lineEditName);
    registerField("proPath*", ui->lineEditPath);

    // 文本变更时触发校验
    connect(ui->lineEditName, &QLineEdit::textEdited, this, &ProSetPage::checkInput);
    connect(ui->lineEditPath, &QLineEdit::textEdited, this, &ProSetPage::checkInput);

    // 设置默认路径为当前目录
    QString curPath = QDir::currentPath();
    ui->lineEditPath->setText(curPath);
    ui->lineEditPath->setCursorPosition(ui->lineEditPath->text().size());

    // 启用右侧清除按钮
    ui->lineEditPath->setClearButtonEnabled(true);
    ui->lineEditName->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage() {
    delete ui;
}

// 通用输入验证函数
ProSetPage::InputStatus ProSetPage::validateInput() const {
    const QString name = ui->lineEditName->text().trimmed();
    const QString path = ui->lineEditPath->text().trimmed();

    if (name.isEmpty() || path.isEmpty()) return EmptyField;

    QDir dir(path);
    if (!dir.exists()) return PathNotExist;

    QString projectPath = dir.absoluteFilePath(name);
    if (QDir(projectPath).exists()) return ProjectExists;

    return Valid;
}

// 根据输入状态实时更新提示
void ProSetPage::checkInput() {
    switch (validateInput()) {
    case EmptyField:
        ui->labelTips->setText("项目名称和路径不能为空");
        break;
    case PathNotExist:
        ui->labelTips->setText("项目路径不存在");
        break;
    case ProjectExists:
        ui->labelTips->setText("该项目路径已存在");
        break;
    case Valid:
        ui->labelTips->setText("");
        break;
    }

    emit completeChanged();
}

// QWizard 使用的有效性判断
bool ProSetPage::isComplete() const {
    // 注意：isComplete() 是一个 const 函数，设计上用于纯粹判断页面是否完整。
    // Qt 框架可能会频繁且不定时调用此函数（甚至在事件循环之外），
    // 因此不应该在这里修改 UI 元素（如调用 label->setText()），
    // 这可能导致界面无法及时刷新，甚至引发未定义行为。
    //
    // 正确的做法是将 UI 提示的更新放到槽函数中（例如 textEdited 触发的 checkInput()），
    // 在那里修改 UI 并发出 completeChanged() 信号通知向导状态变化。
    return validateInput() == Valid;
}

#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QDir>
#include <QFileDialog>
#include <QLineEdit>

ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent), ui(new Ui::ProSetPage) {
    ui->setupUi(this);

    // 注册字段，带 * 表示必须填写
    registerField("projectName*", ui->lineEditName);
    registerField("projectPath*", ui->lineEditPath);

    initUI();
    initSignals();
}

ProSetPage::~ProSetPage() { delete ui; }

void ProSetPage::getProjectSetting(QString &name, QString &dir_path) const {
    name = ui->lineEditName->text().trimmed();
    dir_path = ui->lineEditPath->text().trimmed();
}

// 通用输入验证函数
AppConsts::InputStatus ProSetPage::validateInput() const {
    const QString name = ui->lineEditName->text().trimmed();
    const QString dir_path = ui->lineEditPath->text().trimmed();

    if (name.isEmpty() || dir_path.isEmpty())
        return AppConsts::InputStatus::EmptyField;

    QDir dir(dir_path);
    if (!dir.exists())
        return AppConsts::InputStatus::PathNotExist;

    QString projectPath = dir.absoluteFilePath(name);
    if (QDir(projectPath).exists())
        return AppConsts::InputStatus::ProjectExists;

    return AppConsts::InputStatus::Valid;
}

void ProSetPage::initUI() {
    setTitle(tr("设置项目配置"));
    // 设置提示标签字体颜色为红色
    ui->labelTips->setStyleSheet("color: red;");
    // 设置默认路径为当前目录
    ui->lineEditPath->setText(QDir::currentPath());
    ui->lineEditPath->setCursorPosition(ui->lineEditPath->text().size());
    // 启用右侧清除按钮
    ui->lineEditPath->setClearButtonEnabled(true);
    ui->lineEditName->setClearButtonEnabled(true);
}

void ProSetPage::initSignals() {
    // 文本变更时触发校验
    connect(ui->lineEditName, &QLineEdit::textEdited, this,
            &ProSetPage::onCheckInput);
    connect(ui->lineEditPath, &QLineEdit::textEdited, this,
            &ProSetPage::onCheckInput);
}

void ProSetPage::onCheckInput() {
    switch (validateInput()) {
    case AppConsts::InputStatus::EmptyField:
        ui->labelTips->setText("项目名称和路径不能为空");
        break;
    case AppConsts::InputStatus::PathNotExist:
        ui->labelTips->setText("项目路径不存在");
        break;
    case AppConsts::InputStatus::ProjectExists:
        ui->labelTips->setText("该项目路径已存在");
        break;
    case AppConsts::InputStatus::Valid:
        ui->labelTips->setText("");
        break;
    }

    // 发送信号通知 isComplete()
    emit completeChanged();
}

bool ProSetPage::isComplete() const {
    // 注意：isComplete() 是一个 const 函数，设计上用于纯粹判断页面是否完整。
    // Qt 框架可能会频繁且不定时调用此函数（甚至在事件循环之外），
    // 因此不应该在这里修改 UI 元素（如调用 label->setText()），
    // 这可能导致界面无法及时刷新，甚至引发未定义行为。
    //
    // 正确的做法是将 UI 提示的更新放到槽函数中（例如 textEdited 触发的
    // checkInput()）， 在那里修改 UI 并发出 completeChanged()
    // 信号通知向导状态变化。
    return validateInput() == AppConsts::InputStatus::Valid;
}

void ProSetPage::on_pushButtonBrowse_clicked() {
    // 获取当前 lineEdit 中的路径作为初始目录
    QString dir_path = ui->lineEditPath->text().trimmed();

    // 如果输入为空或目录不存在，则使用当前工作目录作为备选默认路径
    if (dir_path.isEmpty() || !QDir(dir_path).exists()) {
        dir_path = QDir::currentPath();
    }

    // 打开系统文件夹选择对话框，让用户选择一个文件夹路径
    QString selected_dir = QFileDialog::getExistingDirectory(
        this,                                // 父窗口
        tr("选择导入的文件夹"),              // 对话框标题
        dir_path,                            // 默认打开的路径
        QFileDialog::ShowDirsOnly |          // 只显示目录（不显示文件）
            QFileDialog::DontResolveSymlinks // 保留符号链接原样
        );

    // 如果用户取消选择，selectedDir 会是空字符串，此时无需处理
    if (selected_dir.isEmpty())
        return;

    // 将选择的路径填充到 UI 的 lineEditPath 中
    ui->lineEditPath->setText(selected_dir);

    // 立即校验输入，刷新提示和向导按钮状态
    onCheckInput();
}

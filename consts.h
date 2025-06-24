#pragma once
#include <QTreeWidgetItem>

namespace AppConsts {

// 项目树中条目的类型
enum class TreeItemType {
    Project = QTreeWidgetItem::UserType + 1,   // 项目项
    Directory = QTreeWidgetItem::UserType + 2, // 文件夹
    Picture = QTreeWidgetItem::UserType + 3    // 图片
};

// 输入校验状态
// 使用枚举类型 InputStatus 来表示输入校验的不同状态，
// 优点包括：
// 1. 代码语义清晰，返回值更具可读性，不用靠魔法数字或字符串判断结果。
// 2. 方便统一管理各种输入错误状态，便于扩展和维护。
// 3. 避免在多个函数中重复写复杂的判断逻辑，提升代码复用性和一致性。
// 4. 使用 switch 语句处理不同状态时，代码结构更整洁，易于调试和修改。
enum class InputStatus {
    Valid,        // 输入合法，没有错误
    EmptyField,   // 输入字段为空
    PathNotExist, // 指定路径不存在
    ProjectExists // 项目路径已存在，冲突
};

enum class ProjectOperation {
    Create, // 会复制文件且更新 UI
    Import  // 仅更新 UI，不复制
};

enum class ProjectSource {
    Created, // 是创建的
    Imported // 是导入的，不可以提供删除本地文件的功能
};

namespace UIConfig {
constexpr int ProgressWidth = 300;
constexpr int ProgressMax = 300;
} // namespace UIConfig

} // namespace AppConsts

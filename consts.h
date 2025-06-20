#ifndef CONSTS_H
#define CONSTS_H

#endif // CONSTS_H

enum class TreeItemType {
    TreeItemPro = 1, // 条目
    TreeItemDir = 2, // 文件夹
    TreeItemPic = 3  // 图片
};

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

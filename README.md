# 获取TSF输入法候选列表

如果应用程序（如全屏游戏）需要隐藏输入法自身界面，并由自己绘制界面，需要按特定的规则获取输入法候选列表

本示例为TSF框架的输入法候选列表获取提供指导性方法

示例大部分代码复制于微软官方文档，同时保留了部分英文注释。与“获取方法”相关的内容，已用“中文”注释着重说明

基本原理：
  1. 程序启动后，依据TSF框架规范，初始化必要的sink
  2. 输入法使用过程中，在关键事件里回调sink
  3. 按确定的规则在skin回调中获取输入法数据
  
# IMGUI

![](./sdl_imgui.gif)

## 说明
### SDL2
```
版本：SDL version 2.0.9 (stable)
```


## 新知识学习

### Windows batch批处理脚本

- 1.在Windows中使用 `where cl` 来判断 `cl` 是否存在（是否可用）。

- 2.`where cl > nul 2>&1`表示：把STDOUT（1）和STDERR（2）重定向到 `nul`中，即忽略这两个输出。

    - `command > filename 2>&1 `：把标准输出和标准错误一起重定向到一个文件。

    - `command >nul 2>nul`：隐藏程序输出结果。

- 3.`||` ：连接两个命令,当 `||` 前的命令失败时,才执行||后的命令。 

- 4.`call`与`start`命令：
    
    - `call命令`：在同一个cmd.exe中运行。

    - `start命令`：该命令会另外启动一个cmd.exe来运行程序，或者脚本。

### C语言中的#号

- `#` 号：把宏参数变为一个字符串
- `##` 号：把两个宏参数连接起来

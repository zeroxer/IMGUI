# 使用 SDL2 实现IMGUI

## 说明
### SDL2
```
版本：SDL version 2.0.9 (stable)
```


## 新知识学习

### Windows batch批处理脚本

- 1.在Windows中使用 `where cl` 来判断 `cl` 是否存在（是否可用）。

- 2.`>& 2> nul` 是 `1>& 2> nul`的省略形式。表示：把STDOUT（1）和STDERR（2）重定向到 `nul`中，即忽略这两个输出。

- 3.`||` ：连接两个命令,当 `||` 前的命令失败时,才执行||后的命令。 

- 4.`call`与`start`命令：
    
    - `call命令`：在同一个cmd.exe中运行。

    - `start命令`：该命令会另外启动一个cmd.exe来运行程序，或者脚本。

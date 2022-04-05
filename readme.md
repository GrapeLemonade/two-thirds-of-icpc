# two-thirds-of-icpc

## 结对编程项目：单词链求解器

形式上看，结对编程无非就是 2/3 ICPC，因此得名。

`two-thirds-of-icpc` 能够快速地求解单词链，支持以下参数：

- `-n [text file]`：求解单词链总数
- `-w [text file]`：求解最长单词链
- `-m [text file]`：求解首字母不重复的最长单词链
- `-c [text file]`：求解总字符数最多的单词链
- `-h`：限定单词链起始字母
- `-t`：限定单词链末尾字母
- `-r`：允许隐含单词环

此外，本应用还存在 GUI 版本，其可于本仓库 `guibin` 目录下或 [two-thirds-of-icpc-gui](https://github.com/GrapeLemonade/two-thirds-of-icpc-gui) 项目的 Release 中获取。

## 仓库结构

- `CoreExport`：`core.dll` 导出项目
- `bin`：CLI 版本二进制文件目录
- `guibin`：GUI 版本二进制文件目录，未打包
- `src`：源代码
- `test`：测试代码
- `two-thirds-of-icpc-gui`：GUI 开发资源子模块


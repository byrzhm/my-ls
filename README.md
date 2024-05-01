# 我的 ls 实现

## 使用说明

### 处理对象

- 处理对象个数为 0，打印当前目录下所有文件(不含 . 开头的文件)
- 处理对象为普通文件，列出文件
- 处理对象为目录，列出目录下所有文件

### 选项

- r 递归方式列出子目录
- a 列出文件名第一个字符为圆点的普通文件(默认情况下不列出文件名首字符为圆点的文件)
- l 后跟一整数，限定文件大小的最小值(字节)
- h 后跟一整数，限定文件大小的最大值(字节)
- m 后跟一整数n，限定文件的最近修改时间必须在n天内
- -- 显式地终止命令选项分析

## 示例

``` 
> ls *
CMakeLists.txt README.md      ls.c

build:
CMakeCache.txt          CTestTestfile.cmake     cmake_install.cmake
CMakeFiles              DartConfiguration.tcl   compile_commands.json
CPackConfig.cmake       Makefile
CPackSourceConfig.cmake Testing
```

## 参考

- [gnu coreutils](https://www.gnu.org/software/coreutils/manual/html_node/ls-invocation.html#ls-invocation)
- [man7 ls](https://man7.org/linux/man-pages/man1/ls.1.html)
- [getopt](https://man7.org/linux/man-pages/man3/getopt.3.html)

# 我的 ls 实现

## 说明

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

首先初始化目录结构

``` shell
# 创建目录文件
mkdir -p foo/bar
echo a > foo/a.txt
echo b666 > foo/bar/b.txt
echo test > foo/.hidden
```

### 1. 递归方式显示子目录

![None](README.asset/Screenshot%202024-05-01%20220647.png)

### 2. 全部显示, 包括以 `.` 开头的文件

![None](README.asset/Screenshot%202024-05-01%20220909.png)

### 3. 指定文件的大小下限

![None](README.asset/Screenshot%202024-05-01%20221036.png)

### 4. 指定文件的大小上限

![None](README.asset/Screenshot%202024-05-01%20221157.png)

### 5. 限定文件的最近修改时间

![None](README.asset/Screenshot%202024-05-01%20221310.png)

## 参考

- [gnu coreutils](https://www.gnu.org/software/coreutils/manual/html_node/ls-invocation.html#ls-invocation)
- [man7 ls](https://man7.org/linux/man-pages/man1/ls.1.html)
- [getopt](https://man7.org/linux/man-pages/man3/getopt.3.html)

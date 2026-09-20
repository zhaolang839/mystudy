### gcc/g++编译顺序

**gcc -lstdc++ main.cpp** 直接从源代码到目标可执行文件

把过程拆分

​	预处理 gcc -E main.cpp >main.ii 得到main.ii文件

​	编译	gcc -S main.ii 得到main.s的汇编文件

​	汇编	gcc -c main.o 得到main.o(obj)的二进制文件

​	链接	gcc  main.o -o -lstdc++ 得到a.out的可执行文件


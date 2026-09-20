#### Makefile定义

一个工程中的源文件不计其数，其按类型、功能、模块分别放在若干个目录中，Makefile定义了一系列规则来指定那些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作。

#### Make与Makefile的关系

make是一个命令工具，它解释Makefile中的指令。而在Makefile文件中描述了整个工程所有文件的编译顺序、编译规则。

#### Makefile命名规则

Makefile或者makefile，一般使用Makefile

#### Cmake定义与作用

https://www.runoob.com/cmake/cmake-tutorial.html

#### Makefile符号含义

1. @

    这个符号通常用在命令行中，表示不显示命令本身，只显示他的结果。

2. 

#### Makefile基本语法

目标：依赖

Tab	命令/规则

目标：一般指要编译的目标，也可以是一个动作

依赖：指执行当前目标所依赖的先项，包括其他目标，某个具体文件或库等

​			一个目标可以有多个依赖

命令：该目标下要执行的具体命令，可以没有，也可以有多条。多条时，每个命令一行。

##### Makefile基本语法测试1

```makefile
# Makefile文件如下
a:
	@echo "hello world"

# make命令运行结果如下
hello world
```

##### Makefile基本语法测试2

```Makefile
a:
	@echo "hello world"
b:
	@echo "hello"
	
# make b
hello
# make
hello world
```

##### Makefile基本语法测试3

```Makefile
a:b c
	@echo "hello world"
b:
	@echo "hello"
c:
	@echo "hello everyone"

# make
hello
hello everyone
helloworld

### 去掉@符号
a:b c
	echo "hello world"
b:
	@echo "hello"
c:
	@echo "hello everyone"

# make
hello
hello everyone
echo "hello world"
helloworld
# 显示出命令行
```

##### Makefile基本语法测试4

```Makefile
a:
	@echo "helloworld"
	@ls ./
	g++ main.cpp

clean:
	rm -rf a.out
	echo "make clean success"

# 清除生成的a.out文件
```

### Makefile常用选项

**make {-f file}{options}{target}**

make默认在当前目录中国寻找GUNmakefile，make，Mikefile的文件作为make的输入文件

-f 可以指定除上述文件名之外的文件作为输入文件

-v 显示版本号

-n 只输出命令，但并不执行，一般用来测试

-s 只执行命令，但不显示具体的命令，此处可在命令中用@符抑制命令输出

-w 显示执行前执行后的路径

-C dir指定makefile所在的目录



没有指定目标时，默认使用第一个目标

如果只指定，则执行对应的命令

### Makefile中的变量

##### 系统变量

$*	不包括扩展名的目标文件名称

$+	所有的依赖文件，以空格分开

$+	表示规则中的第一个条件

$?	所有时间戳比目标文件晚的依赖文件，以空格分隔

$@  目标文件的完整名称

$^	所有不重复的依赖文件，以空格分隔

$%   如果目标是归档成员，则该变量表示目标的归档成员名称

##### 系统常量（可用 make -p查看）

AS	汇编程序的名称		默认为os

CC	C编译器名称			 默认cc

CPP  C预编译器名称		 默认cc -E

CXX  C++编译器名称		 默认g++

RM   文件删除程序别名    默认 rm -f

​	

##### Makefile基本语法测试5

```Makefile
OBJ=add.o sub.o multi.o
TARGET=calc

$(TARGET):$(OBJ)
	g++ $^ $(TARGET).cpp -o $@

add.o:add.cpp
	g++ -c $^ -o $@

sub.o:sub.cpp
	g++ -c $^ -o $@

multi.o:multi.cpp
	g++ -c $^ -o $@

calc.o:calc.cpp
	g++ -c $^ -o $@

clean:
	rm -rf *.o calc

```

```Makefile
# 采用系统常量替换编译器，达到跨平台开发的目的

OBJ=add.o sub.o multi.o
TARGET=calc

$(TARGET):$(OBJ)
	$(CXX) $^ $(TARGET).cpp -o $@

add.o:add.cpp
	$(CXX) -c $^ -o $@

sub.o:sub.cpp
	$(CXX) -c $^ -o $@

multi.o:multi.cpp
	$(CXX) -c $^ -o $@

calc.o:calc.cpp
	$(CXX) -c $^ -o $@

clean:
	rm -rf *.o calc

show:
	@echo $(AS)
	@echo $(CC)
	@echo $(CPP)
	@echo $(CXX)
	@echo $(RM)

```

##### Makefile中的伪目标和模式匹配

伪目标 .PHONY.clean

​	声明目标为伪目标之后，makefile将不会判断目标是否存在或者该目标是否需要更新。

%.o:%.cpp	.o依赖于对应的.cpp

wildcard		$(wildcard	./*.cpp) 获取当前目录下所有的.cpp文件

patsubst		$(patsubst	%.cpp, %.o, ./*.cpp)将对应的cpp文件名替换成.o文件名

##### Makefile基本语法测试5

```Makefile
# .PHONY:clean show

OBJ=add.o sub.o multi.o
TARGET=calc

$(TARGET):$(OBJ)
	$(CXX) $^ $(TARGET).cpp -o $@

add.o:add.cpp
	$(CXX) -c $^ -o $@

sub.o:sub.cpp
	$(CXX) -c $^ -o $@

multi.o:multi.cpp
	$(CXX) -c $^ -o $@

calc.o:calc.cpp
	$(CXX) -c $^ -o $@

clean:
	rm -rf *.o calc

show:
	@echo $(AS)
	@echo $(CC)
	@echo $(CPP)
	@echo $(CXX)
	@echo $(RM)


.PHONY:clean show

#.PHONY放前放后都可以
```

##### Makefile基本语法测试6

```Makefile
OBJ=add.o sub.o multi.o
TARGET=calc

$(TARGET):$(OBJ)
	$(CXX) $^ $(TARGET).cpp -o $@
	
#模式匹配
%.o:%.cpp
	$(CXX) -c $^ -o $@

clean:
	rm -rf *.o calc

show:
	@echo $(AS)
	@echo $(CC)
	@echo $(CPP)
	@echo $(CXX)
	@echo $(RM)
	@echo $(wildcard	./*.cpp)
	@echo $(patsubst	%.cpp, %.o, ./*.cpp)


.PHONY:clean show
```

#### Makefile执行流程

如下图所示，Makefile文件执行过程遵循递归原则，从最上层开始根据依赖往下执行。

![image-20250621224030783](C:/Users/BlueMoon/AppData/Roaming/Typora/typora-user-images/image-20250621224030783.png)

执行遵循的基本规则：

1. 保证目标是用最新的依赖生成的
2. 第一次完全编译，后面只编译最新的代码（部分编译）



### Makefile 中编译动态链接库 .dll  .so 库文件

动态链接库：不会把代码编译到二进制文件中，而是在运行时才去加载，所以只需要维护一个地址。

-fPIC  产生位置无关的代码

-shared	共享

-l(小L)	指动态库

-I（大i）	指定头文件目录，默认当前目录

-L	手动指定库文件搜索目录，默认只链接共享目录



**动态链接库**	好处是程序可以和库文件分离，可以分别发版，然后库文件可以被多处共享

**动态**	运行时才去加载	动态加载

**链接**	指库文件和二进制文件分离，用某种特殊手段维护二者之间的关系

**库	库文件	.dll 或者 .so**



##### 动态链接库测试1

```makefile

```


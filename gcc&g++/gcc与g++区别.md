[GCC](https://zhida.zhihu.com/search?content_id=110364364&content_type=Article&match_order=1&q=GCC&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NTAzNDI4MzQsInEiOiJHQ0MiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxMTAzNjQzNjQsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.qWkrKu_OFJJyzVvKDX-ETg1x0QJYzCq4GZcV1m8I2Sk&zhida_source=entity) ，[gcc](https://zhida.zhihu.com/search?content_id=110364364&content_type=Article&match_order=1&q=gcc&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NTAzNDI4MzQsInEiOiJnY2MiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxMTAzNjQzNjQsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.RDUhCUzBaqoTkmKI9pYljM0cKu4r0M1BUHGCrWqMuRE&zhida_source=entity) 和[g++](https://zhida.zhihu.com/search?content_id=110364364&content_type=Article&match_order=1&q=g%2B%2B&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NTAzNDI4MzQsInEiOiJnKysiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxMTAzNjQzNjQsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.4ldM0dFpj6zNkuFcPbb9KEYhGk1SZgmwW7TfuxvRNmg&zhida_source=entity)：

一直没搞清这几个东西的概念，搜了半天看到了一个不错的解释，所以大致记录一下，以免以后再忘记，[链接](https://link.zhihu.com/?target=https%3A//www.cnblogs.com/samewang/p/4774180.html)。（原谅没找到原文出处）

GCC:[GNU Compiler Collection](https://zhida.zhihu.com/search?content_id=110364364&content_type=Article&match_order=1&q=GNU+Compiler+Collection&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NTAzNDI4MzQsInEiOiJHTlUgQ29tcGlsZXIgQ29sbGVjdGlvbiIsInpoaWRhX3NvdXJjZSI6ImVudGl0eSIsImNvbnRlbnRfaWQiOjExMDM2NDM2NCwiY29udGVudF90eXBlIjoiQXJ0aWNsZSIsIm1hdGNoX29yZGVyIjoxLCJ6ZF90b2tlbiI6bnVsbH0.18QnTzuhGD14shSaWY6ye2mFctZIHmCON9sIEQg-yxw&zhida_source=entity)(GUN 编译器集合)，它可以编译C、C++、JAV、Fortran、Pascal、Object-C等语言。

gcc是GCC中的GUN [C Compiler](https://zhida.zhihu.com/search?content_id=110364364&content_type=Article&match_order=1&q=C+Compiler&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NTAzNDI4MzQsInEiOiJDIENvbXBpbGVyIiwiemhpZGFfc291cmNlIjoiZW50aXR5IiwiY29udGVudF9pZCI6MTEwMzY0MzY0LCJjb250ZW50X3R5cGUiOiJBcnRpY2xlIiwibWF0Y2hfb3JkZXIiOjEsInpkX3Rva2VuIjpudWxsfQ.U1uGIA0DUQoDB9voHY_evG2yr-kyIdvMlbo-aP3ASBE&zhida_source=entity)（C 编译器）

g++是GCC中的GUN [C++ Compiler](https://zhida.zhihu.com/search?content_id=110364364&content_type=Article&match_order=1&q=C%2B%2B+Compiler&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NTAzNDI4MzQsInEiOiJDKysgQ29tcGlsZXIiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxMTAzNjQzNjQsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.oR6awmWe5YqE9_Ja-iHdlT12B7I3sr5W7GAg0IXwDKI&zhida_source=entity)（C++编译器）



由于编译器是可以更换的，所以gcc不仅仅可以编译C文件

所以，更准确的说法是：gcc调用了C compiler，而g++调用了C++ compiler

gcc和g++的主要区别

1. 对于 *.c和*.cpp文件，gcc分别当做c和cpp文件编译（c和cpp的语法强度是不一样的）

2. 对于 *.c和*.cpp文件，g++则统一当做cpp文件编译

3. 使用g++编译文件时，**g++会自动链接标准库[STL](https://zhida.zhihu.com/search?content_id=110364364&content_type=Article&match_order=1&q=STL&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3NTAzNDI4MzQsInEiOiJTVEwiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxMTAzNjQzNjQsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.eiffmPtY4X7iubAnkUlru3sawpdVNzxtI_s5iLUIG7o&zhida_source=entity)，而gcc不会自动链接STL**

4. gcc在编译C文件时，可使用的预定义宏是比较少的

5. gcc在编译cpp文件时/g++在编译c文件和cpp文件时（这时候gcc和g++调用的都是cpp文件的编译器），会加入一些额外的宏。

   6.在用gcc编译c++文件时，为了能够使用STL，需要加参数 –lstdc++ ，但这并不代表 gcc –lstdc++ 和 g++等价，它们的区别不仅仅是这个。



## [GCC的gcc和g++区别](https://www.cnblogs.com/samewang/p/4774180.html)

看的Linux公社的一篇文章，觉得不错，内容复制过来了。

其实在这之前，我一直以为gcc和g++是一个东西，只是有两个不同的名字而已，今天在linux下编译一个c代码时出现了错误才找了一下gcc和g++的区别。

先描述一下今天遇到的错误：

因为以前写程序的时候，写的都是cpp文件，然后就直接使用g++编译，没出错，也就没有在意过这些问题，今天有一个c文件，但里面包含后STL中的vector语法，想着使用gcc来编译，结果报错了，报错是：找不到vector。

什么是gcc / g++

首先说明：gcc 和 GCC 是两个不同的东西

GCC:GNU Compiler Collection(GUN 编译器集合)，它可以编译C、C++、JAV、Fortran、Pascal、Object-C、Ada等语言。

gcc是GCC中的GUN C Compiler（C 编译器）

g++是GCC中的GUN C++ Compiler（C++编译器）

一个有趣的事实就是，就本质而言，gcc和g++并不是编译器，也不是编译器的集合，它们只是一种驱动器，根据参数中要编译的文件的类型，调用对应的GUN编译器而已，比如，用gcc编译一个c文件的话，会有以下几个步骤：

Step1：Call a preprocessor, like cpp.

Step2：Call an actual compiler, like cc or cc1.

Step3：Call an assembler, like as.

Step4：Call a linker, like ld

由于编译器是可以更换的，所以gcc不仅仅可以编译C文件

所以，更准确的说法是：gcc调用了C compiler，而g++调用了C++ compiler

gcc和g++的主要区别

1. 对于 *.c和*.cpp文件，gcc分别当做c和cpp文件编译（c和cpp的语法强度是不一样的）

2. 对于 *.c和*.cpp文件，g++则统一当做cpp文件编译

3. 使用g++编译文件时，**g++会自动链接标准库STL，而gcc不会自动链接STL**

4. gcc在编译C文件时，可使用的预定义宏是比较少的

5. gcc在编译cpp文件时g++在编译c文件和cpp文件时（这时候gcc和g++调用的都是cpp文件的编译器），会加入一些额外的宏，这些宏如下：

\#define __GXX_WEAK__ 1
		#define __cplusplus 1
		#define __DEPRECATED 1
		#define __GNUG__ 4
		#define __EXCEPTIONS 1
		#define __private_extern__ extern

6. 在用gcc编译c++文件时，为了能够使用STL，需要加参数 –lstdc++ ，但这并不代表 gcc –lstdc++ 和 g++等价，它们的区别不仅仅是这个

主要参数

-g - turn on debugging (so GDB gives morefriendly output)

-Wall - turns on most warnings

-O or -O2 - turn on optimizations

-o - name of the output file

-c - output an object file (.o)

-I - specify an includedirectory

-L - specify a libdirectory

-l - link with librarylib.a

使用示例：g++ -ohelloworld -I/homes/me/randomplace/include helloworld.C
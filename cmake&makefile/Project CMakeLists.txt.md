## 3361工程CMakeLists.txt

```cmake
# 项目根目录设置
set(PROJECT_ROOT_DIR ${ROOT_DIR}/project/project_hima3361)

# 添加依赖子模块（BSP/CORE/OS/CPU），每个模块均有CMakeLists.txt文件，负责该模块的编译（生成静态库或目标文件），供主项目链接。
add_subdirectory(${BSP_DIR} ${BIN_ROOT_DIR}/bsp)
add_subdirectory(${CORE_DIR} ${BIN_ROOT_DIR}/core)
add_subdirectory(${OS_DIR} ${BIN_ROOT_DIR}/os)
add_subdirectory(${CPU_DIR} ${BIN_ROOT_DIR}/cpu)

# 作用：递归查找项目根目录下所有.c/.cpp/.s文件，存储到变量PROJ_SRC中。
# 收集项目源码文件（.c/.cpp/.s）
file(GLOB PROJ_SRC_C ${PROJECT_ROOT_DIR}/*.c)
file(GLOB PROJ_SRC_CPP ${PROJECT_ROOT_DIR}/*.cpp)
file(GLOB PROJ_SRC_ASM ${PROJECT_ROOT_DIR}/*.s)
set(PROJ_SRC ${PROJ_SRC_C} ${PROJ_SRC_CPP} ${PROJ_SRC_ASM})

# 单元测试（UT）配置
if(UT_ENABLE)
    message("${BoldMagenta}Build UT test!!!${ColourReset}")
    add_subdirectory(${CORE_DIR}/utils/3rd/cunit-3.4.4 ${BIN_ROOT_DIR}/unit_test/cunit)  # Cunit测试库
    add_subdirectory(hal_chipest ${BIN_ROOT_DIR}/unit_test/hal_chipest)  # 芯片测试文件
    set(EXTEND_DEFINE "-DPROJ_HIMA3361 -UT_ENABLE=1 ${FE_RUN_MODE}")
    set(EXTEND_SRC_FILE "${HAL_CHIPTEST}")
    set(EXTEND_LINK_FLAG "cunit hal_source")
    set(EXTEND_ELF_NAME "_chitest")
else()
    set(EXTEND_DEFINE "-DPROJ_HIMA3361 ${FE_RUN_MODE}")
    set(EXTEND_SRC_FILE "")
    set(EXTEND_LINK_FLAG "")
    set(EXTEND_ELF_NAME "")
endif()

# 核心0（前端核心，CORE_ID=0）配置
if(CORE_ID STREQUAL "0")
    set(FE_FW "front_end${EXTEND_ELF_NAME}")
    message("${BoldGreen}Build Front End Core${ColourReset}")

    # 编译宏定义（核心ID+扩展定义）
    set(DEFINES "-DCORE_ID=0 ${EXTEND_DEFINE}")
    # 额外编译 flags（内存对齐/代码分节等）
    set(EXTRA_CFLAGS "-mstrict-align -malways-align -fno-jump-tables -ffunction-sections -fdata-sections -fno-common -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-strict-aliasing -fno-builtin -funroll-loops")
    # RISC-V C/C++编译 flags（警告/优化/静态链接）
    set(CMAKE_RISCV32_C_FLAGS "-Wall -Werror -Wno-error=format -Wno-format -02 -static ${DEFINES} ${EXTRA_CFLAGS}")
    set(CMAKE_RISCV32_CXX_FLAGS "${CMAKE_RISCV32_C_FLAGS} -fno-exceptions")

    # 链接器 flags（根据UT_ENABLE选择链接脚本）
    if(UT_ENABLE STREQUAL "ON")
        set(CMAKE_EXE_LINKER_FLAGS "-WL,--mno-execit -WL,-Map=${FE_FW}.map -T${PROJECT_ROOT_DIR}/hima3361_cpu0.ld -WL,--gc-sections -WL,--check-sections -WL,-v -WL,--start-group -WL, --end-group")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "-WL,--mno-execit -WL,-Map=${FE_FW}.map -T${PROJECT_ROOT_DIR}/hima3361_cpu0.ld -WL,--gc-sections -WL,--check-sections -WL,-v -WL,--start-group  -WL, --end-group")
    endif()

    # 生成可执行文件（ELF）
    add_executable(${FE_FW}.elf
        ${PROJ_SRC}
        ${EXTEND_SRC_FILE}
    )

    # 链接依赖库（私有依赖+全归档链接）
    target_link_libraries(${FE_FW}.elf
        PRIVATE
        # 头文件包含
        project_include
        # 依赖库（全归档链接确保符号不丢失）
        -WL,--whole-archive
        ssd cpu bsp os utils
        ${EXTEND_LINK_FLAG}
        -WL,--no-whole-archive
    )

    # 生成辅助文件（bin/dis/sym等）
    add_custom_command(TARGET ${FE_FW}.elf
        COMMAND riscv32-elf-size -B ${FE_FW}.elf  # 查看段大小
        COMMAND riscv32-elf-objcopy -O binary ${FE_FW}.elf ${FE_FW}.bin  # 转二进制镜像
        COMMAND riscv32-elf-objdump -d ${FE_FW}.elf > ${FE_FW}.dis  # 反汇编
        COMMAND riscv32-elf-objdump -t ${FE_FW}.elf > ${FE_FW}.sym  # 符号表
        COMMAND riscv32-elf-objdump -S -l ${FE_FW}.elf > ${FE_FW}.lst  # 带源码反汇编
        COMMAND riscv32-elf-objdump -h ${FE_FW}.elf > ${FE_FW}.sec  # 段信息
        COMMAND riscv32-elf-objdump -x ${FE_FW}.elf > ${FE_FW}.x  # 所有头信息
    )

# 核心1（后端核心，CORE_ID=1）配置
elseif(CORE_ID STREQUAL "1")
    set(BE_FW "back_end${EXTEND_ELF_NAME}")
    message("${BoldGreen}Build Back End Core${ColourReset}")

    # 根据NAND类型设置宏定义
    if(NAND_TYPE STREQUAL "WD8")
        set(DEFINES "-DCORE_ID=1 ${EXTEND_DEFINE} -DNAND_TYPE=2")
    else()
        set(DEFINES "-DCORE_ID=1 ${EXTEND_DEFINE}")
    endif()

    # 编译 flags（同核心0，略）
    set(EXTRA_CFLAGS "-mstrict-align -malways-align -fno-jump-tables -ffunction-sections -fdata-sections -fno-common -fno-omit-frame-pointer -fno-optimize-sibling-calls")
    set(CMAKE_RISCV32_C_FLAGS "-Wall -Werror -Wno-error=format -Wno-format -02 -static ${DEFINES} ${EXTRA_CFLAGS}")
    set(CMAKE_RISCV32_CXX_FLAGS "${CMAKE_RISCV32_C_FLAGS} -fno-exceptions")
    set(CMAKE_ASM_FLAGS "-x assembler-with-cpp -D__ASSEMBLY__" CACHE INTERNAL "")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 ${CMAKE_RISCV32_C_FLAGS}" CACHE INTERNAL "")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG ${CMAKE_RISCV32_C_FLAGS}" CACHE INTERNAL "")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_RISCV32_CXX_FLAGS} ${CMAKE_CXX_FLAGS}" CACHE INTERNAL "")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG ${CMAKE_RISCV32_CXX_FLAGS} ${CMAKE_CXX_FLAGS}" CACHE INTERNAL "")

    # 链接器 flags（使用核心1的链接脚本）
    if(UT_ENABLE STREQUAL "ON")
        set(CMAKE_EXE_LINKER_FLAGS "-WL,--mno-execit -WL,-Map=${BE_FW}.map -T${PROJECT_ROOT_DIR}/hima3361_cpu1_debug.ld -WL,--gc-sections -WL,--check-sections -WL,-v -WL,--start-group")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "-WL,--mno-execit -WL,-Map=${BE_FW}.map -T${PROJECT_ROOT_DIR}/hima3361_cpu1.ld -WL,--gc-sections -WL,--check-sections -WL,-v -WL,--start-group")
    endif()

    # 生成可执行文件（ELF）
    add_executable(${BE_FW}.elf
        ${PROJ_SRC}
        ${EXTEND_SRC_FILE}
    )

    # 链接依赖库（同核心0，略）
    target_link_libraries(${BE_FW}.elf
        PRIVATE
        project_include
        -WL,--whole-archive
        ssd cpu bsp os utils
        ${EXTEND_LINK_FLAG}
        -WL,--no-whole-archive
    )

    # 生成辅助文件（同核心0，略）
    add_custom_command(TARGET ${BE_FW}.elf
        COMMAND riscv32-elf-size -B ${BE_FW}.elf
        COMMAND riscv32-elf-objcopy -O binary ${BE_FW}.elf ${BE_FW}.bin
        COMMAND riscv32-elf-objdump -d ${BE_FW}.elf > ${BE_FW}.dis
        COMMAND riscv32-elf-objdump -t ${BE_FW}.elf > ${BE_FW}.sym
        COMMAND riscv32-elf-objdump -S -l ${BE_FW}.elf > ${BE_FW}.lst
        COMMAND riscv32-elf-objdump -h ${BE_FW}.elf > ${BE_FW}.sec
        COMMAND riscv32-elf-objdump -x ${BE_FW}.elf > ${BE_FW}.x
    )

else()
    message(FATAL_ERROR "Please define CORE_ID in cmake cache variable!")
endif()

# VU使能配置
if(VU_ENABLE)
    message("${BoldMagenta}VU ENABLE=${VU_ENABLE}${ColourReset}")
    add_definitions("-DVU_ENABLE=${VU_ENABLE}")
endif()
```



## CMake内置变量

### **一、提供信息的变量（描述系统/环境）**

| 变量名称                      | 描述                                           |
| ----------------------------- | ---------------------------------------------- |
| `CMAKE_SYSTEM_NAME`           | 操作系统名称（如 Windows, Linux, Darwin）。    |
| `CMAKE_SYSTEM_VERSION`        | 操作系统版本号。                               |
| `CMAKE_SYSTEM_PROCESSOR`      | 系统处理器架构（如 x86_64, arm, aarch64）。    |
| `CMAKE_HOST_SYSTEM_NAME`      | CMake 所在主机的操作系统名称（编译主机系统）。 |
| `CMAKE_HOST_SYSTEM_VERSION`   | 编译主机的操作系统版本。                       |
| `CMAKE_HOST_SYSTEM_PROCESSOR` | 编译主机的处理器架构。                         |
| `CMAKE_MAJOR_VERSION`         | CMake 主版本号。                               |
| `CMAKE_MINOR_VERSION`         | CMake 次版本号。                               |
| `CMAKE_PATCH_VERSION`         | CMake 补丁等级。                               |
| `CMAKE_COMMAND`               | CMake 可执行文件的全路径。                     |

### **二、工程与路径相关变量**

| 变量名称                   | 描述                                                         |
| -------------------------- | ------------------------------------------------------------ |
| `PROJECT_SOURCE_DIR`       | 工程根目录（顶级 `CMakeLists.txt` 所在路径）。               |
| `PROJECT_BINARY_DIR`       | 运行 `cmake` 命令的目录（通常为 `${PROJECT_SOURCE_DIR}/build`）。 |
| `PROJECT_NAME`             | 通过 `project` 命令定义的项目名称。                          |
| `CMAKE_CURRENT_SOURCE_DIR` | 当前处理的 `CMakeLists.txt` 所在路径。                       |
| `CMAKE_CURRENT_BINARY_DIR` | 当前目标的编译目录（存放中间文件）。                         |
| `CMAKE_CURRENT_LIST_DIR`   | 当前 `CMakeLists.txt` 的完整路径。                           |
| `CMAKE_SOURCE_DIR`         | 顶级工程的源码目录（与 `PROJECT_SOURCE_DIR` 相同，优先使用）。 |
| `CMAKE_BINARY_DIR`         | 顶级工程的构建目录（整个项目的根构建目录）。                 |
| `EXECUTABLE_OUTPUT_PATH`   | 可执行文件的存放位置（自定义输出目录）。                     |
| `LIBRARY_OUTPUT_PATH`      | 库文件的存放位置（自定义输出目录）。                         |


### **三、控制编译的变量**

| 变量名称               | 描述                                                         |
| ---------------------- | ------------------------------------------------------------ |
| `CMAKE_C_COMPILER`     | 指定 C 编译器（如 `gcc`）。                                  |
| `CMAKE_CXX_COMPILER`   | 指定 C++ 编译器（如 `g++`）。                                |
| `CMAKE_C_FLAGS`        | C 编译选项（如 `-g`、`-O2`）。                               |
| `CMAKE_CXX_FLAGS`      | C++ 编译选项。                                               |
| `CMAKE_BUILD_TYPE`     | 构建类型（如 `Debug`、`Release`，影响优化和调试信息）。      |
| `CMAKE_INSTALL_PREFIX` | 安装目录前缀（默认 Unix 为 `/usr/local`，Windows 为 `C:\Program Files`）。 |
| `CMAKE_DEBUG_POSTFIX`  | Debug 版本目标文件的后缀（通常设为 `d`，如 `libxxx_d.so`）。 |
| `CMAKE_GENERATOR`      | 编译器生成器名称（如 `Unix Makefiles`、`Visual Studio 7`）。 |

### **四、内部变量（不建议项目中直接使用）**

| 变量名称                         | 描述                                                         |
| -------------------------------- | ------------------------------------------------------------ |
| `CMAKE_HOME_DIRECTORY`           | 源树顶部路径（与 `CMAKE_SOURCE_DIR` 相同，优先使用 `CMAKE_SOURCE_DIR`）。 |
| `CMAKE_INTERNAL_PLATFORM_ABI`    | 内部变量，用于确定编译器 ABI（可能变更）。                   |
| `CMAKE_COMPILER_ABI`             | 内部变量，编译器 ABI 标识（可能变更）。                      |
| `CMAKE_COMPILER_ARCHITECTURE_ID` | 内部变量，编译器目标架构标识（可能变更）。                   |

### **五、其他功能变量**

| 变量名称                  | 描述                                                   |
| ------------------------- | ------------------------------------------------------ |
| `CMAKE_MODULE_PATH`       | 模块查找路径（`find_package` 等命令搜索模块的路径）。  |
| `CMAKE_CURRENT_LIST_FILE` | 当前正在处理的 `CMakeLists.txt` 文件路径（含文件名）。 |
| `CMAKE_CURRENT_LIST_LINE` | 当前 `CMakeLists.txt` 中正在处理的行号。               |



## CMake如何控制项目执行

### **一、文件定位与项目结构**

根据文件夹内容，核心文件位于 `project/hima3361` 目录下，关键文件包括：

- **`.s` 汇编文件**：`start.S`（启动入口）、`trap.S`（异常处理）  
- **`.ld` 链接脚本**：`hima3361_cpu0.ld`（CPU0 内存布局）、`hima3361_cpu1.ld`（CPU1 内存布局）  
- **`.c` 代码文件**：`main.c`（主逻辑）、`startup.c`（初始化）、`trap.c`（异常处理 C 实现）等  

### **二、CMake 对 `.s`/`.c`/`.ld` 文件的调用逻辑**

#### **1. `.s` 汇编文件与 `.c` 文件：通过 `file(GLOB)` 收集并编译**

CMake 通过 **文件通配符** 递归收集 `.s` 和 `.c` 文件，统一纳入编译流程，具体代码对应**CMakeLists.txt**第 12-16 行：  

```cmake
# 收集 .c 文件（如 main.c、startup.c）
file(GLOB PROJ_SRC_C ${PROJECT_ROOT_DIR}/*.c)  
# 收集 .s 汇编文件（如 start.S、trap.S）
file(GLOB PROJ_SRC_ASM ${PROJECT_ROOT_DIR}/*.s)  
# 合并所有源文件到 PROJ_SRC 变量
set(PROJ_SRC ${PROJ_SRC_C} ${PROJ_SRC_ASM})  
```

- **路径映射**：`${PROJECT_ROOT_DIR}` 对应文件夹中的 `project/hima3361` 目录，因此上述命令会匹配该目录下所有 `.c` 和 `.s` 文件。  
- **关键细节**：`start.S` 和 `trap.S` 作为汇编源文件，会被编译为目标文件（`.o`），与 `.c` 文件生成的目标文件一同参与链接。

#### **2. `.ld` 链接脚本：通过链接器标志 `-T` 显式调用**

`.ld` 脚本用于定义程序的 **内存布局**（如代码段、数据段、堆/栈地址），在链接阶段通过 `-T` 选项指定，对应**CMakeLists.txt**第 48/50 行：  

```cmake
# 链接器标志中通过 -T 指定 .ld 脚本路径
set(CMAKE_EXE_LINKER_FLAGS 
  "-WL,--mno-execit -WL,-Map=${FE_FW}.map 
   -T${PROJECT_ROOT_DIR}/hima3361_cpu0.ld  # 调用 CPU0 的链接脚本
   -WL,--gc-sections ..."
)
```

- **路径映射**：`${PROJECT_ROOT_DIR}/hima3361_cpu0.ld` 对应文件夹中 `project/hima3361/hima3361_cpu0.ld`。  
- **选择逻辑**：该项目为多核（如 CPU0/CPU1），可通过条件判断切换脚本（如 `hima3361_cpu1.ld`））。

### **三、编译链接顺序（结合 CMake 执行流程）**

#### **阶段 1：子模块编译（优先于主项目源文件）**

```cmake
# 第 5-8 行：编译 BSP/CORE/OS/CPU 子模块（生成静态库 .a 或目标文件 .o，3361项目子模块中CMakeLists.txt文件是生成静态库，后面全部阐述为子模块库（.a））
add_subdirectory(${BSP_DIR} ${BIN_ROOT_DIR}/bsp)
add_subdirectory(${CORE_DIR} ${BIN_ROOT_DIR}/core)
add_subdirectory(${OS_DIR} ${BIN_ROOT_DIR}/os)
add_subdirectory(${CPU_DIR} ${BIN_ROOT_DIR}/cpu)
```

- **作用**：子模块（如驱动、内核）先编译为库文件，供主项目链接。  
- **顺序**：子模块编译 → 主项目源文件处理（确保主项目能链接子模块的库）。

#### **阶段 2：主项目源文件编译（`.s` 与 `.c` 并行处理）**

- **输入**：`PROJ_SRC` 变量（含 `.c` 和 `.s` 文件）  
- **编译命令**：  
  - `.c` 文件：通过 `riscv32-elf-gcc` 编译为目标文件（如 `main.o`、`startup.o`），使用 `CMAKE_RISCV32_C_FLAGS` 中的编译选项（如 `-02`、`-ffunction-sections`）。  
  - `.s` 汇编文件：通过 `riscv32-elf-as` 编译为目标文件（如 `start.o`、`trap.o`），汇编器会解析 `start.S` 中的指令（如设置异常向量、异常栈初始化）。  


#### **阶段 3：链接阶段（`.ld` 脚本主导内存布局）**

- **输入**：所有目标文件（`.o`）+ 子模块库（`.a`）+ `.ld` 脚本  
- **链接命令**：`riscv32-elf-ld -T hima3361_cpu0.ld ...`（由 `CMAKE_EXE_LINKER_FLAGS` 生成）  
- **`.ld` 脚本作用**：  
  1. 定义 **入口点**：通常在 `.ld` 中通过 `ENTRY(_start)` 指定程序入口为 `start.S` 中的 `_start` 符号。  
  2. 分配段地址：例如将 `.text` 段（代码）放在 `0x80000000`，`.data` 段（数据）放在 `0x80100000`。  
  3. 链接顺序：`.ld` 脚本中的 `SECTIONS` 指令决定目标文件的合并顺序，`start.o`（汇编）通常被放在最前面。  

#### **阶段 4：生成可执行文件与辅助文件**

```cmake
# 第 53-57 行：生成 ELF 可执行文件
add_executable(${FE_FW}.elf ${PROJ_SRC} ${EXTEND_SRC_FILE})  
# 第 72-80 行：生成 bin/hex/map 等文件（如 front_end.bin）
add_custom_command(TARGET ${FE_FW}.elf POST_BUILD  
  COMMAND riscv32-elf-objcopy -O binary ${FE_FW}.elf ${FE_FW}.bin  
)
```

### **四、运行时执行顺序（结合汇编与 C 代码）**

#### **1. 第一步：`start.S` 汇编代码（最先执行，硬件初始化）**

- **触发点**：`.ld` 脚本通过 `ENTRY(_start)` 指定入口为 `start.S` 中的 `_start` 符号，CPU 复位后从该地址开始执行。  

- **核心功能**：  

  ```asm
  .section .text.startup.asm "ax"
  .global _start
  _start:
    la t0, _stack  # 初始化栈指针（栈地址由 .ld 脚本定义）
    call startup_c  # 调用 C 函数（startup.c 中的 startup_c函数）
  ```

  - 初始化栈（依赖 `.ld` 脚本中定义的 `_stack` 地址）。  
  - 跳转到 C 代码入口 `startup_c`（位于 `startup.c`）。  

#### **2. 第二步：`startup.c`（C 语言初始化）**

- **调用链**：`start.S::_start` → `startup.c::startup_c`  

- **核心功能**：初始化全局变量、硬件外设（如 UART、中断控制器），最终调用 `main.c` 中的 `main` 函数：  

  ```c
  void startup_c() {
    cpu_startup_disable_cache();  	// 禁用CPU缓存
    startup_c_section_init();     	// 初始化 C 语言段（.data 和 .bss 段）
    startup_clib_init();		   	    // 初始化 C 标准库
    startup_core_init();				// 初始化内核
      
    main();              				// 跳转到主逻辑
  }
  ```


#### **3. 第三步：`main.c`（业务逻辑）**

- **调用链**：`startup_c` → `main`  

- **核心功能**：执行用户业务代码，例如：  

  ```c
  int main() {
    printf("Hello, hima3361!\n");  // 依赖 stub_hal.c 中的 HAL 库实现
    while(1);
  }
  ```


#### **4. 异常处理：`trap.S` 与 `trap.c` 协同**

- **触发场景**：中断或异常发生时，CPU 跳转到 `trap.S` 中的异常入口（由 `.ld` 脚本的 `exception_vector` 段定义）。  
- **执行流程**：  
  1. `trap.S` 保存上下文（寄存器），调用 `trap.c::handle_trap` 处理异常。  
  2. 处理完成后，`trap.S` 恢复上下文并返回原程序。  

### **五、关键依赖关系总结**

| 文件类型      | CMake 调用方式                 | 执行顺序（运行时）                       | 依赖项                                     |
| ------------- | ------------------------------ | ---------------------------------------- | ------------------------------------------ |
| `.ld` 脚本    | 链接阶段通过 `-T` 选项指定     | 编译期决定内存布局，运行时无执行         | 需在链接前存在，定义 `_start` 入口和栈地址 |
| `.s` 汇编文件 | `file(GLOB)` 收集后编译为 `.o` | 最先执行（`start.S::_start`）            | 依赖 `.ld` 脚本定义的符号（如 `_stack`）   |
| `.c` 文件     | `file(GLOB)` 收集后编译为 `.o` | 汇编初始化后执行（`startup_c` → `main`） | 依赖 `.s` 提供的栈和硬件初始化             |

### **六、验证与调试**

1. **查看编译日志**：确认 `.s` 和 `.c` 文件是否被正确编译（如 `[ 50%] Building ASM object start.o`）。  
2. **检查链接脚本路径**：通过 `message(STATUS "Linker script: ${PROJECT_ROOT_DIR}/hima3361_cpu0.ld")` 打印路径，确保指向 `project/hima3361` 目录。  
3. **反汇编验证入口**：通过 `riscv32-elf-objdump -d front_end.elf` 查看反汇编，确认 `_start` 符号对应 `start.S` 代码。  

通过以上流程，CMake 实现了 `.s`/`.c` 文件的自动化编译和 `.ld` 脚本的内存布局控制，最终形成从 **汇编启动→C 初始化→业务逻辑** 的完整执行链。


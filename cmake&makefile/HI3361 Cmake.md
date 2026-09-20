## HI3361工程cmake编译流程

### 整体代码架构如下：

```
.
├── bsp
│   ├── bsp_hima3361
│   │   ├── bsp_nfi
│   │   ├── bsp_nvme
│   │   ├── bsp_sata
│   │   ├── bsp_soc
│   │   └── hal
│   │          ├── atm
│   │          ├── bfc
│   │          ├── ddr
│   │          ├── dnacc
│   │          ├── ftl
│   │          ├── nfi
│   │          ├── nvme
│   │          ├── pcie
│   │          ├── sata
│   │          ├── soc
│   │          └── hdma
│   └── bsp_simu
│       ├── bsp_vnfi
│       ├── bsp_vnvme
│       ├── bsp_vsata
│       └── bsp_vsoc
├── build
├── cmake
├── core
│   ├── 3rd
│   │   ├── cmocka
│   │   ├── fff
│   │   ├── googletest
│   │   ├── json
│   │   ├── lettershell
│   │   ├── lz4
│   │   └── sfud
│   ├── be
│   ├── fe
│   │   ├── nvme
│   │   │   ├── admin
│   │   │   ├── io
│   │   │   ├── mi
│   │   │   └── vu
│   │   └── sata
│   │       ├── admin
│   │       └── io
│   ├── ftl
│   ├── lib
│   │   └── stdio
│   └── sys
│       └── cli
├── cpu
│   ├── cpu_hima3361
│   │   ├── andes_n45
│   │   ├── cache
│   │   ├── core
│   │   ├── plic
│   │   ├── pma
│   │   └── timer
│   └── cpu_simu
├── doc
├── os
│   ├── filesystem
│   │   ├── littlefS
│   │   └── yaffs2
│   ├── os_amp
│   	└── freertos
│       	├── include
│       	├── MemMang
│       	└── portable
│           	├── MSVC-MingW
│           	├── portable_hima3361
│           	│   └── tracer
│           	├── portable_sim
│           	└── ThirdParty
│               	└── GCC
│                   	└── Posix
│                      		└── utils
├── os_smp
├── project
│   ├── project_chip_test
│   │   └── hal_chiptest
│   │       ├── atm
│   │       ├── bfc
│   │       ├── ddr
│   │       ├── dnacc
│   │       ├── ftl
│   │       ├── nfi
│   │       ├── nvme
│   │       ├── pcie
│   │       ├── ram
│   │       ├── sata
│   │       ├── soc
│   │       └── hdma
│   ├── project_core_test
│   │   ├── be_test
│   │   ├── config
│   │   ├── fake
│   │   ├── fe_test
│   │   ├── ftl_test
│   │   ├── lib_test
│   │   └── sys_test
│   └── project_hima3361
│       ├── config
│       └── start
└── tools
```

### bash命令启动CMake

项目通过bash命令启动编译，由人为在终端输入，或者直接由脚本运行调用。本项目通过运行bash命令完成CMake**规则设置**与**编译**。

#### **bash命令**：

```bash
cmake --preset <name-of-preset>
cmake --build --preset <name-of-preset>

# <name-of-preset>为ut时：编译出软件ut
# <name-of-preset>为official时：编译出fw bin
# <name-of-preset>为chiptest时：编译出芯片ut
```

- **第一条命令**：通过预设配置生成构建系统（Makefile），完成 “准备工作”；
- **第二条命令**：调用 `make` 执行编译链接，将源代码转化为可执行固件，核心依赖预设中定义的工具链、编译参数和项目配置。

### 阶段1 ：`cmake --preset <name-of-preset>`（配置阶段）

`cmake --preset <name-of-preset>` 本质是**配置过程**，不直接执行编译（编译由后续的 `--build` 完成），但它是编译的前提。其核心是根据预设 `<name-of-preset>` 的规则，生成构建系统（如 Makefile）。

#### 1. 查找并解析预设文件

CMake 会在当前目录（或父目录）查找预设文件（优先级：`CMakeUserPresets.json` > `CMakePresets.json`），并定位到名为 `<name-of-preset>` 的预设配置。

`<name-of-preset>`其中`official_core0_nvme`配置代码如下：

```json
{
  "name": "official_core0_nvme",
  "displayName": "official_core0_nvme",
  "description": "configure for official core0 nvme fw project",
  "generator": "Unix Makefiles",
  "binaryDir": "build/${presetName}",
  "cacheVariables": {
    "CMAKE_BUILD_TYPE": "Debug",
    "PROJ_TAG": "PROJ_HI3361",
    "CORE_ID": "0",
    "CMAKE_VERBOSE_MAKEFILE": "OFF",
    "CMAKE_TOOLCHAIN_FILE": "${sourceDir}/cmake/riscv.cmake",
    "FE_RUN_MODE": "-DFE_RUN_MODE=1 -DLOOP_MODE=3 -DENCRY_SWITCH_MODE=1"
  }
}
```

预设文件中 `<name-of-preset>` 的配置包含：

- 构建目录（`binaryDir`，如 `build/<name-of-preset>`）：存放生成的构建文件和后续编译产物，这里会将`official_core0_nvme`替代`<name-of-preset>`。

- 源码目录（`sourceDir`）：指定 `CMakeLists.txt` 所在的根目录，此处未设置，为默认当前路径。

- 生成器（`generator`）：指定构建系统类型`Unix Makefiles`。

- 缓存变量（`cacheVariables`）： `CMAKE_BUILD_TYPE=Debug`：调试模式，`PROJ_TAG`：项目标签，`CORE_ID`：核心 ID 为 `0`，`CMAKE_VERBOSE_MAKEFILE== OFF`：关闭详细构建日志，`CMAKE_TOOLCHAIN_FILE`：指定交叉编译工具链文件（`riscv.cmake`），用于 RISC-V 架构的编译。

  - **riscv.cmake**代码如下：

    ```cmake
    set(CMAKE_SYSTEM_NAME Generic)    # 设置系统名为 Generic（通用嵌入式系统）
    set(CMAKE_SYSTEM_VERSION "5")     # 设置系统版本为 5
    set(CMAKE_SYSTEM_PROCESSOR riscv32)  # 指定处理器架构为 32 位 RISC-V
    set(CMAKE_CROSSCOMPILING ON)      # 启用交叉编译标志
    
    # 工具链三元组和目标编译器配置
    set(triple riscv32-unknown-elf)  # 目标平台格式：架构-厂商-操作系统（ELF格式）
    set(CMAKE_CXX_COMPILER riscv32-elf-g++)    # C++编译器
    set(CMAKE_C_COMPILER riscv32-elf-gcc)      # C编译器
    set(CMAKE_ASM_COMPILER riscv32-elf-gcc)    # 汇编编译器（复用GCC）
    set(CMAKE_LINKER riscv32-elf-gcc)          # 链接器（复用GCC）
    
    # 编译器选项
    add_compile_options(
        -mcpu=n45                     # 指定CPU架构为 AndesCore N45
        -mtune=andes-45-series        # 针对N45系列处理器优化
        -mcmodel=medany               # 内存模型：支持最大2GB寻址（LUI+ADDI指令加载地址）
    )
    
    # 链接器选项
    add_link_options(
        -mcpu=n45                     # 保持与编译器一致的架构
        -mtune=andes-45-series        # 保持与编译器一致的优化
        -nostartfiles                 # 禁用标准启动文件，需自定义_start入口和硬件初始化
    )
    ```

- `FE_RUN_MODE`：自定义宏定义，传递以下参数：
  
  - `-DFE_RUN_MODE=1`：启用某种运行模式——暂时不清楚待定。
  - `-DLOOP_MODE=3`：设置循环模式为 `3`。
  - `-DENCRY_SWITCH_MODE=1`：启用加密开关（可能是拼写错误，应为 `ENCRYPT_SWITCH_MODE`）。

#### 2. 验证编译环境

根据**riscv.cmake**文件中预设的工具链和生成器，CMake 会：

- 检查指定的编译器是否存在（如 `gcc`、`g++`），并验证版本是否符合要求。
- 检查依赖库是否可用（ `CMakeLists.txt` 中声明了依赖）。
- 验证系统环境（如操作系统、架构是否匹配预设的目标平台）。

若环境不满足（如编译器缺失），配置会失败并输出错误信息（如 `Could not find CMAKE_C_COMPILER`）。

#### 3. 解析 `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.12)

# 1. 定义终端颜色控制序列（用于彩色输出日志）
# 获取 ASCII 转义字符（终端控制序列的起始标记）
string(ASCII 27 Esc)
# 定义常用颜色和样式的 ANSI 转义序列
set(ColourReset "${Esc}[0m")    # 重置颜色/样式
set(Red         "${Esc}[31m")   # 红色文字
set(Green       "${Esc}[32m")   # 绿色文字
set(Yellow      "${Esc}[33m")   # 黄色文字
set(Blue        "${Esc}[34m")   # 蓝色文字
set(Magenta     "${Esc}[35m")   # 品红文字
set(Cyan        "${Esc}[36m")   # 青色文字
set(White       "${Esc}[37m")   # 白色文字
set(BoldRed     "${Esc}[1;31m") # 加粗红色
set(BoldGreen   "${Esc}[1;32m") # 加粗绿色
set(BoldYellow  "${Esc}[1;33m") # 加粗黄色
set(BoldBlue    "${Esc}[1;34m") # 加粗蓝色
set(BoldMagenta "${Esc}[1;35m") # 加粗品红
set(BoldCyan    "${Esc}[1;36m") # 加粗青色
set(BoldWhite   "${Esc}[1;37m") # 加粗白色


# 2. 执行 Git 命令获取版本信息
# 2.1 获取 Git 提交哈希（含 dirty 标记，标识代码是否有未提交修改）
exec_program(
    "git"                     # 要执行的外部程序：git
    ${CMAKE_CURRENT_SOURCE_DIR}  # 执行目录：当前 CMakeLists.txt 所在的源码根目录
    ARGS "describe --always --dirty --abbrev=8"  # Git 参数：生成 8 位哈希+dirty 标记
    OUTPUT_VARIABLE GIT_SHA1  # 存储命令输出结果（提交哈希信息）
    RETURN_VALUE GIT_RETURN   # 存储命令返回值（0 表示成功，非 0 表示失败）
)

# 2.2 获取当前 Git 分支名称
exec_program(
    "git"                     # 要执行的外部程序：git
    ${CMAKE_CURRENT_SOURCE_DIR}  # 执行目录：源码根目录
    ARGS "rev-parse --abbrev-ref HEAD"  # Git 参数：解析当前分支名
    OUTPUT_VARIABLE GIT_BRANCH  # 存储命令输出结果（分支名称）
    RETURN_VALUE GIT_RETURN   # 存储命令返回值
)


# 3. 处理 Git 版本信息并定义宏（供代码中使用）
if(0 EQUAL ${GIT_RETURN})  # 判断 Git 命令是否执行成功（返回值为 0）
    message("git-sha: " ${GIT_SHA1})       # 输出提交哈希信息
    add_definitions(-DGIT_SHA1="${GIT_SHA1}")  # 定义 C 宏，代码中可通过 GIT_SHA1 访问
    message("git-branch: " ${GIT_BRANCH})   # 输出分支名称
    add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")  # 定义 C 宏，代码中可通过 GIT_BRANCH 访问
else()
    # 若 Git 命令失败（如非 Git 仓库），宏值设为 null
    add_definitions(-DGIT_SHA1="null")
    add_definitions(-DGIT_BRANCH="null")
endif()


# 4. 生成构建时间戳并定义宏
string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")  # 生成带时分秒的时间（如 2025-08-05 15:30:20）
string(TIMESTAMP BUILD_DATE "%Y%m%d")              # 生成纯日期（如 20250805）

add_definitions(-DBUILD_TIME="${BUILD_TIME}")  # 定义构建时间宏
add_definitions(-DBUILD_DATE="${BUILD_DATE}")  # 定义构建日期宏

# 彩色输出构建时间（使用前面定义的颜色变量）
message("${BoldGreen}Build Time: ${BUILD_TIME}${ColourReset}")
message("${BoldGreen}Build Date: ${BUILD_DATE}${ColourReset}")


# 5. 项目基础配置
project(HI3361)  # 定义项目名称（后续可通过 ${PROJECT_NAME} 引用）

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # 生成 compile_commands.json（供 IDE 索引和静态分析）

# 定义核心目录变量（统一管理路径，避免硬编码）
set(ROOT_DIR ${PROJECT_SOURCE_DIR})        # 源码根目录
set(BIN_ROOT_DIR ${PROJECT_BINARY_DIR})    # 构建产物输出目录（编译中间文件、可执行程序等）

# 适配 CYGWIN 环境（Windows 下的类 Unix 环境）：转换路径格式（如 C:/ → /cygdrive/c/）
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "CYGWIN")
    string(REPLACE "C:" "/cygdrive/c" ROOT_DIR ${ROOT_DIR})
    string(REPLACE "D:" "/cygdrive/d" ROOT_DIR ${ROOT_DIR})
    string(REPLACE "C:" "/cygdrive/c" BIN_ROOT_DIR ${BIN_ROOT_DIR})
    string(REPLACE "D:" "/cygdrive/d" BIN_ROOT_DIR ${BIN_ROOT_DIR})
endif()

# 定义子模块目录（按项目结构拆分，方便后续引用）
set(OS_DIR ${ROOT_DIR}/os)        # 操作系统相关代码目录
set(CORE_DIR ${ROOT_DIR}/core)    # 核心功能代码目录
set(CPU_DIR ${ROOT_DIR}/cpu)      # CPU 相关代码目录
set(BSP_DIR ${ROOT_DIR}/bsp)      # 板级支持包目录
set(PROJ_ROOT_DIR ${ROOT_DIR}/project)  # 项目入口目录


# 6. 引入外部编译和链接配置（复用通用规则）
include(cmake/compile_link.cmake)  # 包含外部 CMake 脚本


# 7. 定义头文件搜索路径（通过接口库传递给依赖目标）， 创建一个名为 project_include 的接口库
add_library(project_include INTERFACE)  # 创建接口库（仅用于传递头文件路径和编译选项）
target_include_directories(project_include  # 为接口库设置头文件目录
    INTERFACE 
    ${PROJ_ROOT_DIR}  # 任何依赖该库的目标，自动将此目录加入头文件搜索路径
)


# 8. 根据 PROJ_TAG 选择编译目标（多项目分支支持）
if(PROJ_TAG STREQUAL "PROJ_HI3361")
    # 分支 1：编译 HI3361 硬件目标
    project(${PROJECT_NAME} C ASM)  # 声明支持的语言：C 和汇编（硬件项目可能需要汇编）
    set(PROJECT_ROOT_DIR ${ROOT_DIR}/project/project_hima3361)  # 覆盖项目根目录为硬件项目目录
    add_subdirectory(project/project_hima3361)  # 加入硬件项目的子目录（执行其 CMakeLists.txt）

elseif(PROJ_TAG STREQUAL "PROJ_SIMU")
    # 分支 2：编译仿真目标
    project(${PROJECT_NAME} C)  # 声明支持的语言：仅 C（仿真项目可能无需汇编）
    enable_testing()  # 开启测试支持（配合 CTest 框架运行单元测试）
    add_subdirectory(project/project_simu)  # 加入仿真项目的子目录

else()
    # 分支 3：无有效目标时报错
    message(FATAL_ERROR "No specific PROJECT NAME: ${PROJECT_NAME}")  # 终止构建并提示错误
endif()

```

**compile_link.cmake代码如下：**

```cmake
# help function
macro(SUBDIRLISTINCLUDE result gurdir)    # 定义Cmake宏 SUBDIRLISTINCLUDE，输出result、gurdir为屏幕集的起始目录
set(dirList "")								# dirList初始化为空
file(GLDB_RECURSE DIRS_WITH_HEADERS "$(gurdir)/*.N")		# 通过搜索GUI地址下的全部.h的库文件，DIRS_WITH_HEADERS存储结果，GLDB_RECURSE递归搜索
foreach(header ${DIRS_WITH_HEADERS})		# 遍历全部头文件路径
get_filename_component(dir ${header} DIRECTORY)		# 完整文件路径中提取目录部分，DIRECTORY指定提取目录部分
list(APPEND dirList ${dir})			# 将提取的目录添加到列表
endforeach()
list(REMOVE_DUPLICATES dirList)		# 去重
set($(result) ${dirList})			# 返回给result
endmacro()


# 用于在构建系统中添加一个文件转换规则
function(generate_ld input_file output_file)		# 定义了一个名为 generate_ld 的 CMake 函数，接收两个参数
    if(NOT EXISTS ${input_file})					# 检查输入文件是否存在
        message(STATUS "Input file ${input_file} does not exist.")
    endif()
    # 打印状态信息，显示正在处理的文件和生成的目标文件
    message(STATUS "Processing file: ${input_file} to generate ${output_file}")
    
	# 创建一个自定义构建规则
	# 确保在构建过程中自动运行该命令，当输入文件更新时重新生成输出文件
	# 在构建过程中，当 input.lds 文件更新时，自动运行 nds_ldsag input.lds -o output.ld 命令
	add_custom_command(
        OUTPUT ${output_file}		# 指定生成的输出文件
        COMMAND nds_ldsag ${input_file} -o ${output_file}		# 定义要执行的命令
        DEPENDS ${input_file}		# 声明输入文件作为依赖
        COMMENT "Generating ${output_file} from ${input_file}"	# 构建时显示的描述信息
    )
    # 再次执行命令（可删除该语句），在配置阶段生成文件
    execute_process(COMMAND nds_ldsag ${input_file} -o ${output_file})
endfunction()

```

CMake 会从源码目录的根 `CMakeLists.txt` 开始，**递归解析**所有子目录的 `CMakeLists.txt`，执行其中的指令：

- 处理项目信息（`project()` 指令，定义项目名、版本等）。
- 解析编译选项（`add_compile_options()`，如 `-Wall` 开启警告、`-g` 生成调试符号）。
- 识别目标（`add_executable()` 定义单元测试可执行程序，`add_library()` 定义依赖库）。
- 处理目标依赖（`target_link_libraries()` 链接测试框架或其他库）。
- 解析测试相关配置（如 `enable_testing()`、`add_test()` 定义单元测试用例）。

#### 4. 生成 CMake 缓存（CMakeCache.txt）

配置过程中，CMake 会将所有关键配置（如编译器路径、缓存变量、依赖路径等）写入构建目录下的 `CMakeCache.txt` 文件。该文件相当于 “配置快照”，记录了构建系统生成的关键参数，后续重新配置时会优先读取缓存（可通过 `--fresh` 选项强制刷新缓存，如 `cmake --preset ut --fresh`）。

#### 5. 生成构建系统文件

根据预设的 `generator`（如 `Unix Makefiles`），CMake 在 `binaryDir`（如 `build/ut`）中生成对应的构建系统文件：

- 若为 `Unix Makefiles`：生成 `Makefile`、`CMakeFiles/` 目录（包含中间配置）。
- 若为 `Ninja`：生成 `build.ninja`、`rules.ninja` 等脚本。
- 若为 `Visual Studio`：生成 `.sln` 解决方案和 `.vcxproj` 项目文件。

这些文件包含了详细的编译规则（如源文件列表、编译器参数、链接顺序等），是后续 `cmake --build` 执行编译的依据。

#### 6. 输出配置结果

配置成功后，CMake 会输出总结信息，包括：

- 构建目录路径（`-- Build files have been written to: ...`）。
- 使用的编译器（如 `C compiler: /usr/bin/gcc (GNU 11.2.0)`）。
- 关键缓存变量（如 `CMAKE_BUILD_TYPE: Debug`）。

若配置失败（如依赖缺失、语法错误），会输出具体错误位置（如 `CMakeLists.txt:10 (add_executable): Unknown target`），需修复后重新执行配置。

### 阶段2：`cmake --build --preset <name-of-preset>`（构建阶段）

该命令是 **编译构建阶段**，用于调用构建工具（如 `make`）执行实际的编译、链接，生成最终产物（如固件镜像）。流程如下：

#### 1. **定位构建目录与构建工具**

CMake 根据预设 `official_core0_nvme` 找到 `binaryDir`（`build/official_core0_nvme`），并确认该目录已存在有效的构建系统文件（如 `Makefile`）。由于生成器是 `Unix Makefiles`，CMake 会自动调用系统默认的构建工具 `make`（可通过 `CMAKE_MAKE_PROGRAM` 自定义，此处未指定则用默认 `make`）。

#### 2. **调用 `make` 工具执行构建**

`make` 进入 `build/official_core0_nvme` 目录，读取 `Makefile` 并执行默认目标（通常为 `all`，即构建所有输出产物）。

#### 3. **编译阶段：源文件 → 目标文件（.o）**

`make` 根据 `Makefile` 中的依赖规则，对项目源文件（`.c`/`.cpp`）进行编译，生成目标文件（`.o`）。关键细节如下：

- **编译器与工具链**：
  由工具链文件 `riscv.cmake` 指定交叉编译器（如 `riscv-elf-gcc`），而非系统默认编译器。工具链文件还会注入 RISC-V 架构相关参数，例如：

  ```bash
  riscv-elf-gcc -march=rv32imac -mabi=ilp32 -g -O0 ...  # 架构、ABI、调试信息、无优化（Debug模式）  
  ```

- **编译选项与宏定义**：
  结合配置阶段的缓存变量，生成具体编译命令：

  - `CMAKE_BUILD_TYPE=Debug`：添加 `-g`（调试信息）、`-O0`（无优化）；
  - `PROJ_TAG=PROJ_HI3361`、`CORE_ID=0`：通过 `add_definitions` 或 `target_compile_definitions` 转化为 `-DPROJ_HI3361 -DCORE_ID=0`，用于代码中的条件编译（如 `#ifdef PROJ_HI3361`）；
  - `FE_RUN_MODE`：直接传递 `-DFE_RUN_MODE=1 -DLOOP_MODE=3 -DENCRY_SWITCH_MODE=1`，控制功能运行模式（如加密开关、循环逻辑）；
  - 头文件路径：`CMakeLists.txt` 中通过 `include_directories` 指定的路径会转化为 `-I<path>`，确保编译器能找到头文件。

- **目标文件存储**：
  编译生成的 `.o` 文件暂存于 `build/official_core0_nvme/CMakeFiles/<target_name>.dir/` 目录（`<target_name>` 为项目定义的目标名，如 `core0_fw`）。

#### 4. **链接阶段：目标文件 → 最终产物**

所有目标文件（`.o`）编译完成后，`make` 调用链接器（由工具链文件指定，如 `riscv-none-elf-ld`）将其链接为最终产物（如固件镜像 `.elf`、`.bin`）。关键细节：

- **链接脚本**：
  嵌入式项目需通过链接脚本（`.ld`）定义内存布局（如代码段、数据段地址），工具链文件 `riscv.cmake` 通常会通过 `-Tlinker.ld` 指定链接脚本路径（如 `cmake/linker_riscv.ld`）。
- **依赖库链接**：
  若项目依赖外部库（如数学库 `libm`、自定义静态库 `.a`），`Makefile` 会根据 `CMakeLists.txt` 中的 `target_link_libraries` 指令，添加 `-lm`（链接数学库）或 `-L<lib_path> -l<lib_name>`（链接自定义库）。
- **输出产物**：
  链接生成的主产物通常是 `.elf` 文件（含调试信息的可执行文件），存于 `build/official_core0_nvme` 目录。若项目配置了 `add_custom_command`（如通过 `objcopy` 转换格式），还会生成 `.bin`（二进制镜像，用于烧录）或 `.hex` 文件。

#### 5. **构建进度与结果**

- **输出信息**：由于 `CMAKE_VERBOSE_MAKEFILE=OFF`，`make` 仅显示简要进度（如 `[ 50%] Building C object ...`），不打印完整编译命令（如需详细命令，可添加 `--verbose` 选项：`cmake --build --preset ... --verbose`）。
- **成功标志**：若所有编译、链接步骤无错误，`make` 退出并返回 0，最终产物（如 `.elf`、`.bin`）生成在 `build/official_core0_nvme` 目录；若失败，`make` 会输出错误信息（如语法错误、未定义符号）并终止。

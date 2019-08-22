#!/bin/bash

set -e

export OPTIMIZE="-Oz -Wall -Werror --llvm-lto 1 -fno-exceptions"
export LDFLAGS="${OPTIMIZE}"
export CFLAGS="${OPTIMIZE}"
export CXXFLAGS="${OPTIMIZE}"

echo -e "\033[?25l" # 隐藏光标
echo -e "\033[44;37m ============================================= \033[0m"
echo ''
echo -e "\033[32m[ wasm ] 开始构建   \033[0m"
echo -e "\033[1A"
(
  # Compile C/C++ code
  docker run --rm -it -v `pwd`:/src apiaryio/emcc emcc \
    ./src-wasm/main.cpp \
    ${OPTIMIZE} \
    -s MALLOC=emmalloc \
    -s EXPORT_ES6=0 \
    -s SINGLE_FILE=1 \
    -s INLINING_LIMIT=0 \
    -s NO\_FILESYSTEM=1 \
    -s ASSERTIONS=1 \
    -s NO_EXIT_RUNTIME=1  \
    -s WASM=0 \
    -s ENVIRONMENT="web" \
    --memory-init-file 0 \
    --bind \
    -o build/main-asm.js
)

    
#  -s MODULARIZE=1
#  -s ENVIRONMENT="web"  

#  -s STRICT=1   删除对所有已弃用的构建选项的支持。这可确保您的代码以向前兼容的方式构建。
#  -s ALLOW_MEMORY_GROWTH=1   允许内存在必要时自动增长
#  -s EXPORT_ES6=1   将JavaScript代码转换为ES6模块，其默认导出适用于任何捆绑器
#  -s FILESYSTEM=0  是否启用文件系统

echo -e "\033[32m[ wasm ] 构建完成   \033[0m"
echo -e "\033[32m[ Path ] ./build/main-asm.js   \033[0m"
echo ''
echo -e "\033[44;37m ============================================= \033[0m"
echo -e "\033[?25h" # 显示光标

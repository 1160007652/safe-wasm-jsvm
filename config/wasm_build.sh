#!/bin/bash

set -e

# 编译quickjs 根目录
export QUICKJSHOME="./lib/quickjs-2020-03-16"

export inputFile="./src-wasm/main.cpp ${QUICKJSHOME}/quickjs.c ${QUICKJSHOME}/cutils.c ${QUICKJSHOME}/libregexp.c ${QUICKJSHOME}/libbf.c ${QUICKJSHOME}/libunicode.c"

# export inputFile="./src-wasm/main.cpp"


export outputFile="./public/build/main-wasm.js"

export OPTIMIZE="-DCONFIG_VERSION=\"1.0.0\" -lm -Oz -Wall -Werror --llvm-lto 1 -fno-exceptions"
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
    ${inputFile} \
    ${OPTIMIZE} \
    -s MALLOC=emmalloc \
    -s STRICT=1 \
    -s EXPORT_ES6=0 \
    -s SINGLE_FILE=1 \
    -s INLINING_LIMIT=0 \
    -s NO\_FILESYSTEM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=1 \
    -s NO_EXIT_RUNTIME=0  \
    -s WASM=1 \
    -s ENVIRONMENT="web" \
    -s EXPORT_NAME='SafeJSModule' \
    -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
    --memory-init-file 0 \
    --bind \
    -o ${outputFile}
)

#  -s MALLOC=emmalloc
#  -s FILESYSTEM=0
#  -s STRICT=1   删除对所有已弃用的构建选项的支持。这可确保您的代码以向前兼容的方式构建。
#  -s ALLOW_MEMORY_GROWTH=1   允许内存在必要时自动增长
#  -s EXPORT_ES6=1   将JavaScript代码转换为ES6模块，其默认导出适用于任何捆绑器
#  -s FILESYSTEM=0  是否启用文件系统
#  --cpuprofiler cpu 分析器
#  --memoryprofiler 内存 分析器
#  --threadprofiler 多线程 分析器

echo -e "\033[32m[ wasm ] 构建完成   \033[0m"
echo -e "\033[32m[ Path ] ${outputFile}   \033[0m"

echo -e "\033[32m[ wasm ] Gzip压缩   \033[0m"
echo -e "\033[1A"
(
  gzip -n -c ${outputFile} > ${outputFile}.gz
)
echo -e "\033[32m[ Path ] ${outputFile}.gz   \033[0m"

echo ''
echo -e "\033[44;37m ============================================= \033[0m"
echo -e "\033[?25h" # 显示光标
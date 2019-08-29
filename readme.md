
#### wasm 兼容性：

https://caniuse.com/#feat=wasm

-----------------------
#### 启动 server 服务：

> 安装依赖：npm i 
> 运行服务：npm run server

---------------------

#### wasm转wast
1. 首先安装wasm2wat库
2. wasm2wat main.wasm -o main.wast 

----------------------

#### wasm 编译命令

1. 首先安装 docker 容器
    > https://www.docker.com/
2. 安装基于 docker 的 [emcc](https://hub.docker.com/r/apiaryio/emcc) 镜像
    > docker pull apiaryio/emcc
3. 开始编译   
    > 执行 wasm 编译命令： npm run wasm

    > 执行 asm  编译命令： npm run asm

------------------------

#### 目录结构
- public/              // 启动服务运行的目录
- public/build/        // 保存构建成功的main-[wasm|asm].[js,js.gz]文件
- public/js/           // js代码目录
- public/index.html    // 入口
- config/              // 配置项目录
- config/asm_build.sh  // asm脚本命令
- config/wasm_build.sh // wasm脚本命令
- src-wasm/            // C++源码目录
- src-wasm/main.cpp    // wasm 源码 

#### CPU\内存分析视图

切换到 profiler 分支, 运行查看

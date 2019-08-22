
#### wasm 兼容性：

https://caniuse.com/#feat=wasm

-----------------------
#### 启动 server 服务：

python -m http.server  8088

> 也可以使用其它服务，去运行

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
- build     // 保存构建成功的main-[wasm|asm].js文件
- config    // 用于构建wasm 的命令配置项
- src-fe    // html+js Demo 页面
- src-wasm  // [c|c++] wasm 源码
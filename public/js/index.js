function loadScript(url,callback = undefined) {
    var script=document.createElement("script");
    script.type="text/javascript";
    if(script.readyState){  // ie游览器
        script.onreadystatechange=function () {
            if(script.readyState=="loaded" || script.readyState=="complete"){
                script.onreadystatechange=null;
                callback && callback();
            }
        }
    }else { //其他
        script.onload=function () {
            callback && callback();
        }
    };
    script.src=url;
    document.getElementsByTagName("head")[0].appendChild(script);
}

if (window.WebAssembly && false) {
    // 支持WASM浏览器的使用方式
    loadScript("../build/main-wasm.js");
} else {
    // 不支持WASM浏览器的使用方式
    loadScript("//cdn.bootcss.com/babel-polyfill/7.4.4/polyfill.min.js", function(){
        loadScript("../build/main-asm.js");
    });
}
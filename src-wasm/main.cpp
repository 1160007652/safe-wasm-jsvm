/**
 * @author 刘智攀
 * @email zhipan.liu@okcoin.net
 * @create date 2019-07-24 16:50:27
 * @modify date 2019-07-24 16:50:27
 * @desc net 接口请求 加密
 */

#include <emscripten.h>
#include <string.h>
#include "../lib/quickjs-2020-03-16/quickjs.h"

#ifndef EM_PORT_API
#    if defined(__EMSCRIPTEN__)
#        if defined(__cplusplus)
#            define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#        else
#            define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#        endif
#    else
#        if defined(__cplusplus)
#            define EM_PORT_API(rettype) extern "C" rettype
#        else
#            define EM_PORT_API(rettype) rettype
#        endif
#    endif
#endif

// 导出 api 供 js 调用
EM_PORT_API(const char*) eval(const char* str) {
	
	// 运行时环境
    JSRuntime* runtime = JS_NewRuntime();

	// 获取运行上下文
	JSContext* ctx = JS_NewContext(runtime);

	// 在此上下文中eval 执行js 代码
	JSValue result = JS_Eval(ctx, str, strlen(str), "<evalScript>", JS_EVAL_TYPE_GLOBAL);

	// 异常情况
	if (JS_IsException(result)) {
		JSValue realException = JS_GetException(ctx);
		return JS_ToCString(ctx, realException);
	}
	
	JSValue json = JS_JSONStringify(ctx, result, JS_UNDEFINED, JS_UNDEFINED);
	
	// 释放内存
	JS_FreeValue(ctx, result);
	
    return JS_ToCString(ctx, json);

}

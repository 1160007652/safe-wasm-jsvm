/**
 * @author 刘智攀
 * @email zhipan.liu@okcoin.net
 * @create date 2019-07-24 16:50:27
 * @modify date 2019-07-24 16:50:27
 * @desc net 接口请求 加密
 */

#include <emscripten/val.h>
#include <emscripten.h>
#include <regex>

using namespace emscripten;
using namespace std;

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

// 字符串 反转
string reverseSting(string str){
    char* begin =(char*) str.c_str();
    char *end =(char*) str.c_str() + str.size() - 1;
    while (begin < end)
    {
        swap(*begin, *end);
        begin++;
        end--;
    }
    return str;
}

// 字符串 切割
void splitString(const string& s, vector<string>& v, const string& c){
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

// 根据当前域名生成一个密文
string toSuperBase(string host) {

    string _$_3_ = "btoa";
 
    // base64 正则匹配 == -- start
    string str("=+$");
    regex pattern(str,regex::icase);
    // -- end

    string encoded = val::global(_$_3_.c_str())(val(host)).as<string>();
    encoded = regex_replace(encoded.c_str(), pattern, "$1$3");


    char newEncoded[encoded.length()];
    strcpy(newEncoded,encoded.c_str());

    string flag = "";

    for(int i=0 ; i < encoded.length() ; i++){

        bool isEven = i % 2 == 0;
        char temp = newEncoded[i];
        // 对奇数索引的数字 做差值处理
        if (newEncoded[i] >='0' && newEncoded[i]<='9') {
            temp = isEven ? temp : (char)(9 - (int)( temp - '0' ) + '0' );
        }

        // 对偶数索引的子母 做大小写转换
        if (isalpha(newEncoded[i]) != 0 && isEven) {
            
            int code = (int)temp;

            if (code >= 97) {
                code -= 32;
            } else {
                code += 32;
            }

            temp = (char) code;

        }
        flag = flag + temp;
    }
    return flag;
}

// 生成flag
string generateFlag(string version) {

    char newVersion[version.length()];
    strcpy(newVersion,version.c_str());
    string flag = "";

    for(int i=version.length() -1 ; i >= 0 ; i--){
         char vers = (char)( abs(9 - i -1 - (int)(newVersion[i]-'0')) +'0');
         flag = flag + vers;
    }

    return flag;
}

// 奇偶位交换
string getOddSecret(string secret) {

    char newSecret[secret.length()];
    strcpy(newSecret,secret.c_str());
    string flag = "";

    for(int i=0 ; i < secret.length() -1 ; i+= 2){
        flag = flag + newSecret[i+1] + newSecret[i];
    }
    return flag;
}

// 获取加密cookie
string getServerCookie(string cookie, string hostname) {

    if (cookie == "") {
        return cookie;
    }
    //  字符串定义 正则 "\\d{3}"， \\ 双写 ， 如果是 char类型定义， 只需要 一个数组即可
    char red3[6] = { '\\' , 'd', '{', '3', '}', '\0' };

    string str(red3);
    regex pattern(str,regex::icase);
    
    vector<string> cookies; // 切割过后的数据

    splitString(cookie, cookies,"."); // 切割cookie,将结果存入 cookies.

    bool hasFlag = false;
    if (cookies.size() > 2) {
        hasFlag = regex_search(cookies[0].c_str(), pattern) && regex_search(cookies[1].c_str(), pattern);
    }
    
    if (hasFlag){
        return cookie;
    }

     // 'version.secret'
    string version = cookies[0].c_str();
    string secret = cookies[1].c_str();

    // 根据version生成flag
    string flag = generateFlag(version);

    // 进行奇偶位交换
    string secretAry = getOddSecret(secret); 
    
    // 根据当前域名生成一个密文
    string hostSuperBase = toSuperBase(hostname); // www.okex.me

    // 在索引为5处插入当前域名密文
    secretAry.insert(5,hostSuperBase);

    string hostLengthAry = to_string(hostSuperBase.length());

    string hostLengthCodeAry;

    // 补齐3位
    for (int i=0; i < 3 - hostLengthAry.length() ; i++) {
         hostLengthAry = '0' + hostLengthAry;
    }

    // 表示当前域名密文长度的3个字母 (首位大写 中间位数字不变 第三位小写)
    for (int i=0; i < hostLengthAry.length() ; i++) {
        int item = atoi(hostLengthAry.substr(i, 1).c_str());
        if (i == 0) {
            hostLengthCodeAry += (char)(97 + item);
        } else if (i == 1) {
            hostLengthCodeAry +=  to_string(item);
        } else {
            hostLengthCodeAry +=  (char)(65 + item);
        }
    }
    // 在前面加入当前域名密文长度字符
    secretAry = hostLengthCodeAry + secretAry;
    // 反转字符串
    secretAry = reverseSting(secretAry);

    return flag+"."+version+"."+secretAry;
}

// 加密执行
string runEncode () {
    
    char _location[9] = {'l', 'o', 'c', 'a', 't', 'i', 'o', 'n', '\0' };
    char _hostname[9] = {'h', 'o', 's', 't', 'n', 'a', 'm', 'e', '\0'};
    char _document[9] = {'d', 'o', 'c', 'u', 'm', 'e', 'n', 't', '\0'};
    char _cookie[7] = {'c', 'o', 'o', 'k', 'i', 'e', '\0'};
    char _pvtag[20] = { 'P', 'V', 'T', 'A', 'G', '=', '(', '[', '^', ';', ']', '*', ')', '[', ';', '|', '$', ']', '*', '\0' };

    val location = val::global(_location);
    val document = val::global(_document);

    std::string hostName = location[_hostname].as<std::string>();
    std::string cookie = document[_cookie].as<std::string>();
    
    string str(_pvtag);
    regex pattern(str, regex::icase);
    smatch result;

    if (regex_search(cookie, result, pattern)) {
        cookie = result.format("$1");
    } else {
        cookie = "";
    }
    
    // printf("ho-st-Na-me: %s \n", hostName.c_str());
    // printf("Co-ok-ie  PV-TA-G: %s \n", cookie.c_str());

    return getServerCookie(cookie, hostName);
}

// 导出 api 供 js 调用
EM_PORT_API(const char*) get_js_runEncode() {
    return runEncode().c_str();
}

regex pattern("使用当前时间戳,与服务端的时间进行校验，采用非对称加密 ajax.get() ajax.post $s net", regex::icase);
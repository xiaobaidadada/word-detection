#include "sensitive_struct.h"
#include <node_api.h>
#include <napi.h>

extern p_sensitive_word_node root ;

wchar_t* ConvertNapiStringToWchar(const Napi::String& napiString) {
    // 获取 UTF-8 字符串
    std::string utf8String = napiString.Utf8Value();

    // 将 UTF-8 字符串转换为 std::wstring
    std::wstring wideString(utf8String.begin(), utf8String.end());

    // 分配内存
    wchar_t* wideCharStr = new wchar_t[wideString.size() + 1]; // +1 for null terminator
    std::copy(wideString.begin(), wideString.end(), wideCharStr);
    wideCharStr[wideString.size()] = L'\0'; // 添加结束符

    return wideCharStr;
}

void add_word(const Napi::CallbackInfo &info) {
    // 全局对象
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    Napi::String word = info[0].As<Napi::String>();
    auto str = ConvertNapiStringToWchar(word);
    add_sensitive_word(str);
    delete [] str;
}

Napi::Boolean check_word(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    Napi::String word = info[0].As<Napi::String>();
    auto str = ConvertNapiStringToWchar(word);
    auto r = check_sensitive_word(str);
    delete [] str;
    return Napi::Boolean::New(env, r);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // 初始化根节点
    root = (p_sensitive_word_node) malloc(sizeof (sensitive_word_node));
    create_children(root,first_children_length);
    root->children_num = 0;
    // 设置函数
    exports.Set(Napi::String::New(env, "add_word"),
                Napi::Function::New(env, add_word));
    exports.Set(Napi::String::New(env, "check_word"),
                Napi::Function::New(env, check_word));
    return exports;
}

// 模块的入口点，node addon 就是写模块的
// 名字 初始化函数
NODE_API_MODULE(get_process, Init)





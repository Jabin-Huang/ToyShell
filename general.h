#pragma once
typedef char* sh_string_func_t(char*);  //定义一个函数类型，该函数用于复制字符串
typedef void sh_free_func_t(void*);  //定义一个函数类型，该函数用于释放内存

#define DEFAULT_STRLEN 80
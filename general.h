#pragma once
typedef char* sh_string_func_t(char*);  //����һ���������ͣ��ú������ڸ����ַ���
typedef void sh_free_func_t(void*);  //����һ���������ͣ��ú��������ͷ��ڴ�

#define DEFAULT_STRLEN 80

#if !defined(savestring)
#define savestring(x) (char*)strcpy(malloc(1 + strlen(x)), (x))
#endif
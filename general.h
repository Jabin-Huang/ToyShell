#pragma once
typedef char* sh_string_func_t(char*);  //����һ���������ͣ��ú������ڸ����ַ���
typedef void sh_free_func_t(void*);  //����һ���������ͣ��ú��������ͷ��ڴ�

#define DEFAULT_STRLEN 80
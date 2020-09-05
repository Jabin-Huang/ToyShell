#include <stdlib.h>
#include "exec.h"



#include "command.h"
#include "eval.h"
#include "variable.h"
#include "general.h"

void exec_command(COMMAND* cmd) {
  switch (cmd->type) {
    case cm_assign:
      exec_assign(cmd);
      break;
    case cm_if:
      exec_if(cmd);
      break;
    case cm_while:
      exec_while(cmd);
      break;
    case cm_until:
      exec_until(cmd);
      break;
    case cm_for:
      exec_for(cmd);
      break;
    case cm_simple:
      exec_simple(cmd);
      break;
    default:
      exit(-1);
  }
}

void exec_assign(COMMAND* cmd) {
  ASSIGN_COM* assign = cmd->value.Assign;
  Var* new = (Var*)malloc(sizeof(Var));
  new->name = assign->name;
  if (assign->type == _INT) {
    new->value.exp = *((int*)cal(assign->exp, 0));
  } else {
    new->value.str = assign->exp;
  }
  new->type = assign->type;
  var_update(assign->name, new);
}

void exec_if(COMMAND* cmd) {
  IF_COM* if_com = cmd->value.If;
  COMMAND* c = NULL;
  if (*((int*)cal(if_com->test, 0))) {
    c = if_com->true_case;
  } else {
    c = if_com->false_case;
  }
  while (c != NULL) {
    exec_command(c);
    c = c->next;
  }
}

void exec_while(COMMAND* cmd) {
  WHILE_COM* while_com = cmd->value.While;
  COMMAND* c = NULL;
  while (*((int*)cal(while_com->test, 0))) {
    c = while_com->action;
    while (c != NULL) {
      exec_command(c);
      c = c->next;
    }
  }
}

void exec_until(COMMAND* cmd) {
  UNTIL_COM* until_com = cmd->value.Until;
  do {
    COMMAND* c = until_com->action;
    while (c != NULL) {
      exec_command(c);
      c = c->next;
    }
  } while (!(*(int*)cal(until_com->test, 0)));
}

void exec_for(COMMAND* cmd) { 
    FOR_COM* for_com = cmd->value.For; 
    for (int i = 0; i < for_com->len_list; ++i) {
      Var* v = var_get(for_com->var);
      v->value.str = savestring(for_com->map_list[i]);
      COMMAND* c = for_com->action;
      while (c != NULL) {
        exec_command(c);
        c = c->next;
      }
    }
}

void exec_simple(COMMAND* cmd) { 
    SIMPLE_COM* simple_com = cmd->value.Simple;
    if (strcmp(simple_com->name, "echo") == 0) {
      if ((simple_com->in)[0][0] == '$') {
         char* var = &((simple_com->in)[0][1]);
         Var* v = var_get(var);
         if (v->type == _INT)
           printf("%d\n", v->value.exp);
         else
           printf("%s\n", v->value.str);
	  } else 
		  printf("%s\n", (simple_com->in)[0]);
    } else if (strcmp(simple_com->name, "mkdir") == 0) {
      char* target = (simple_com->in)[0];
      createDirectory(target);

    } else if (strcmp(simple_com->name, "touch") == 0) {
      char* target = (simple_com->in)[0];
      createFile(target);
    }

    else if (strcmp(simple_com->name, "pwd") == 0) {
      getCurDirectory();
    } else if (strcmp(simple_com->name, "cat") == 0) {
      char* target = (simple_com->in)[0];
      readFile(target);
    } else if (strcmp(simple_com->name, "rm") == 0) {
      if (strcmp((simple_com->in)[0], "-f")) {
        char* target = (simple_com->in)[0];
        deleteFile(target);
      } else {
      }
    } else if (strcmp(simple_com->name, "find") == 0) {
      if (strcmp((simple_com->in)[0], "-r") == 0) {
        char* regex = (simple_com->in)[1];
        findfile(regex);
      } else if (strcmp((simple_com->in)[0], "-t") == 0) {
        char* regex = (simple_com->in)[1];
        findfile3(regex);
      } else if (strcmp((simple_com->in)[0], "-k") == 0) {
        char* regex = (simple_com->in)[1];
        findfile2(regex);
      } else {
      }
    } else if (strcmp(simple_com->name, "ls") == 0) {
      curdirlist();
    } else if (strcmp(simple_com->name, "rmdir") == 0) {
      char* target = (simple_com->in)[0];
      removeDirectory(target);
    } else if (strcmp(simple_com->name, "cp") == 0) {
      char* source = (simple_com->in)[0];
      char* target = (simple_com->in)[1];
      fileCopy(source, target);
    } else if (strcmp(simple_com->name, "date") == 0) {
      getTime();
    } else if (strcmp(simple_com->name, "ps") == 0) {
      processShow();
    } else if (strcmp(simple_com->name, "set") == 0) {
      char* variable = (simple_com->in)[0];
      char* value = (simple_com->in)[1];
      setenviable(variable, value);
    } else if (strcmp(simple_com->name, "cd") == 0) {
      char* target = (simple_com->in)[0];
      setcurrentdirectory(target);
    } else if (strcmp(simple_com->name, "cls") == 0) {
      cls();
    } else if (strcmp(simple_com->name, "pause") == 0) {
      pauseP();
    } else if (strcmp(simple_com->name, "enviable") == 0) {
      char* target = (simple_com->in)[0];
      environShow(target);
    } else if (strcmp(simple_com->name, "rename") == 0) {
      char* source = (simple_com->in)[0];
      char* target = (simple_com->in)[1];
      fileReName(source, target);
    } else if (strcmp(simple_com->name, "mv") == 0) {
      char* source = (simple_com->in)[0];
      char* target = (simple_com->in)[1];
      movefile(source, target);
    } else if (strcmp(simple_com->name, "help") == 0) {
      helprint();
    }
}

char* infoTransform(char* str) {
  printf("%s\n", str);
  return str;
}

//创建新目录
// mkdir directory √
void createDirectory(const char* target) {
  char* infostr1 = "create failed";
  int isSuccess;
  isSuccess = CreateDirectoryA(target, NULL);
  if (!isSuccess) infoTransform(infostr1);
}

//创建新文件
// touch filename √
void createFile(const char* target) {
  char* infostr1 = "file already existed, create failed\n";
  char* infostr2 = "create failed\n";
  int isSuccess;
  HANDLE existFile = CreateFileA(target, GENERIC_READ, FILE_SHARE_READ, NULL,
                                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (existFile != INVALID_HANDLE_VALUE) {
    infoTransform(infostr1);
  } else {
    HANDLE newFile = CreateFileA(target, GENERIC_READ, FILE_SHARE_READ, NULL,
                                 CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (newFile == INVALID_HANDLE_VALUE)
      infoTransform(infostr2);
    CloseHandle(newFile);
  }

  CloseHandle(existFile);
}

//读取文件内容
// read filename √
void readFile(const char* target) {
  char* infostr1 = "file not existed/file open failed\n";
  char* infostr2 = "readfile failed\n";
  char* infostr3 = "file detail:\n";
  
  int isSuccess;
  printf("target: %s\n", target);
  HANDLE newFile = CreateFileA(target, GENERIC_READ, FILE_SHARE_READ, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (newFile == INVALID_HANDLE_VALUE) {
    infoTransform(infostr1);
  } else {
    int file_size = 0;
    file_size = GetFileSize(newFile, NULL);
    char* buff;
    buff = (char*)malloc(file_size);
    DWORD dwRead;
    if (!ReadFile(newFile, buff, file_size, &dwRead, NULL)) {
      infoTransform(infostr2);
    }
    buff[file_size] = '\0';
    char* infostr4 = malloc(strlen(infostr3) + strlen(buff) + 1);
    strcpy_s(infostr4, strlen(infostr3)+1, infostr3);
    strcat_s(infostr4, strlen(infostr4) + strlen(buff) + 1, buff);
    infoTransform(infostr4);
    CloseHandle(newFile);
  }
}

//删除文件
// rm filename √
void deleteFile(const char* target) {
  char* infostr1 = "can't find ";
  char* infostr2 = "delete failed, the file might be using or need authority ";
  int isSuccess;
  HANDLE existFile = CreateFileA(target, GENERIC_READ, FILE_SHARE_READ, NULL,
                                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (existFile == INVALID_HANDLE_VALUE) {
    char* infostr4 = malloc(strlen(infostr1) + strlen(target) + 1);
    strcpy_s(infostr4, strlen(infostr1) + 1, infostr1);
    strcat_s(infostr4, strlen(infostr4) + strlen(target) + 1, target);
    infoTransform(infostr4);

    printf("can't find %s\n", target);
    CloseHandle(existFile);
  }

  else {
    CloseHandle(existFile);
    isSuccess = DeleteFileA(target);
    if (!isSuccess) infoTransform(infostr2);
  }
}

//在当前目录下根据正则表达式查找文件（目录+文件）
// find -r regex

void findfile(const char* regex) {
  struct _finddata_t data;
  printf("regex:%s\n", regex);
  intptr_t hnd = _findfirst(regex, &data);
  if (hnd < 0) {
    perror(regex);
  }
  int nRet = (hnd < 0) ? -1 : 1;
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  WORD wOldColorAttrs;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  // Save the current color
  GetConsoleScreenBufferInfo(h, &csbiInfo);
  wOldColorAttrs = csbiInfo.wAttributes;

  while (nRet >= 0) {
    if (data.attrib == _A_SUBDIR) {
      SetConsoleTextAttribute(h, FOREGROUND_BLUE);
      printf("%s/ \n", data.name);
      SetConsoleTextAttribute(h, wOldColorAttrs);
    }

    else {
      SetConsoleTextAttribute(h, FOREGROUND_GREEN);
      printf("%s \n", data.name);
      SetConsoleTextAttribute(h, wOldColorAttrs);
    }
    nRet = _findnext(hnd, &data);
  }
  _findclose(hnd);
}

//在当前目录下查找相应类型的文件（目录+文件）
// find -t type

void findfile3(const char* type) {
  struct _finddata_t data;
  struct _finddata_t test;
  char* regex3 = "*.";
  char* regex4 = malloc(strlen(regex3) + strlen(type) + 1);
  strcpy_s(regex4, strlen(regex3)  + 1, regex3);
  strcat_s(regex4, strlen(regex4) + strlen(type) + 1, type);
  // printf("regex4: %s\n",regex4);
  intptr_t hnd = _findfirst(regex4, &data);
  if (hnd < 0) {
    perror(regex4);
  }
  int nRet = (hnd < 0) ? -1 : 1;
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  WORD wOldColorAttrs;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  // Save the current color
  GetConsoleScreenBufferInfo(h, &csbiInfo);
  wOldColorAttrs = csbiInfo.wAttributes;

  while (nRet >= 0) {
    if (data.attrib == _A_SUBDIR) {
      SetConsoleTextAttribute(h, FOREGROUND_BLUE);
      printf("%s/ \n", data.name);
      SetConsoleTextAttribute(h, wOldColorAttrs);
    }

    else {
      SetConsoleTextAttribute(h, FOREGROUND_GREEN);
      printf("%s \n", data.name);
      SetConsoleTextAttribute(h, wOldColorAttrs);
    }
    nRet = _findnext(hnd, &data);
  }
  _findclose(hnd);
}

//在当前目录下根据关键字查找文件（目录+文件）
// find -k key
void findfile2(char* key) {
  struct _finddata_t data;
  char* regex2 = "*";
  char* regex5 = "*.*";
  // printf("sizeof(regex2):%d\n", sizeof(regex2));
  char* regex4 = malloc(strlen(regex2) + strlen(key) + strlen(regex5) + 1);
  strcpy_s(regex4, strlen(regex2) + 1,regex2);
  strcat_s(regex4, strlen(regex4)+strlen(key) + 1, key);
  strcat_s(regex4, strlen(regex4) + strlen(regex5) + 1,  regex5);
  // strcat_s(regex2,16, key);
  // strcat_s(regex2, 16, "*.*");

  // printf("regex4:%s\n", regex4);

  intptr_t hnd = _findfirst(regex4, &data);
  if (hnd < 0) {
    perror(regex4);
  }
  int nRet = (hnd < 0) ? -1 : 1;
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  WORD wOldColorAttrs;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  // Save the current color
  GetConsoleScreenBufferInfo(h, &csbiInfo);
  wOldColorAttrs = csbiInfo.wAttributes;

  while (nRet >= 0) {
    if (data.attrib == _A_SUBDIR) {
      SetConsoleTextAttribute(h, FOREGROUND_BLUE);
      printf("%s/ \n", data.name);
      SetConsoleTextAttribute(h, wOldColorAttrs);
    }

    else {
      SetConsoleTextAttribute(h, FOREGROUND_GREEN);
      printf("%s \n", data.name);
      SetConsoleTextAttribute(h, wOldColorAttrs);
    }
    nRet = _findnext(hnd, &data);
  }
  _findclose(hnd);
}

//列出当前目录下的所有文件（目录+文件）
// ls
void curdirlist() {
  struct _finddata_t data;
  intptr_t hnd = _findfirst("*", &data);
  if (hnd < 0) {
    perror("*");
  }
  int nRet = (hnd < 0) ? -1 : 1;
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  WORD wOldColorAttrs;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  // Save the current color
  GetConsoleScreenBufferInfo(h, &csbiInfo);
  wOldColorAttrs = csbiInfo.wAttributes;

  while (nRet >= 0) {
    if (data.attrib == _A_SUBDIR) {
      SetConsoleTextAttribute(h, FOREGROUND_BLUE);
      printf("%s/ \n", data.name);
      SetConsoleTextAttribute(h, wOldColorAttrs);
    }

    else {
      SetConsoleTextAttribute(h, FOREGROUND_GREEN);
      printf("%s \n", data.name);
      SetConsoleTextAttribute(h, wOldColorAttrs);
    }
    nRet = _findnext(hnd, &data);
  }
  _findclose(hnd);
}

/*
暂时废除
//列出指定目录下的所有文件（目录+文件）
// ls directory


void dirlist(const char* path) {
        struct _finddata_t data;
        long hnd = _findfirst(path, &data);
        if (hnd < 0) {
                perror(path);
        }
        int nRet = (hnd < 0) ? -1 : 1;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD wOldColorAttrs;
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        // Save the current color
        GetConsoleScreenBufferInfo(h, &csbiInfo);
        wOldColorAttrs = csbiInfo.wAttributes;

        while (nRet >= 0) {


                if (data.attrib == _A_SUBDIR) {
                        SetConsoleTextAttribute(h, FOREGROUND_BLUE);
                        printf("%s/ \n", data.name);
                        SetConsoleTextAttribute(h, wOldColorAttrs);
                }

                else {
                        SetConsoleTextAttribute(h, FOREGROUND_GREEN);
                        printf("%s \n", data.name);
                        SetConsoleTextAttribute(h, wOldColorAttrs);
                }
                nRet = _findnext(hnd, &data);

        }
        _findclose(hnd);
}
*/

//删除目录（有且仅当目录为空）
// rmdir directory √
void removeDirectory(const char* target) {
  char* infostr1 = "delete failed, the directory might not empty";
  int isSuccess;
  isSuccess = RemoveDirectoryA(target);
  if (!isSuccess) infoTransform(infostr1);
}

//复制文件
// cp -f sourcefile targetfile √
void fileCopy(const char* source, const char* target) {
  char* infostr1 = "source path and target path can't be same";
  char* infostr2 = "copy failed";
  int isSuccess;
  int whetherTwo = 0;
  if (source == target)
    infoTransform(infostr1);
  else {
    isSuccess = CopyFileA(source, target, whetherTwo);
    if (!isSuccess) infoTransform(infostr2);
  }
}

//移动文件
// mv  sourcefile targetfile √
void movefile(const char* source, const char* target) {
  char* infostr1 = "move failed\n";
  int isSuccess;
  if (source == target)
    return;
  else {
    isSuccess = MoveFileA(source, target);
    if (!isSuccess) infoTransform(infostr1);
  }
}

//重命名文件
// rename -f preName LaterName √
void fileReName(const char* source, const char* target) {
  char* infostr1 = "can't find ";
  char* infostr2 = "rename failed";
  int isSuccess;
  HANDLE existFile = CreateFileA(source, GENERIC_READ, FILE_SHARE_READ, NULL,
                                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (existFile == INVALID_HANDLE_VALUE) {
    char* infostr4 = malloc(strlen(infostr1) + strlen(target) + 1);
    strcpy_s(infostr4, strlen(infostr1) + 1, infostr1);
    strcat_s(infostr4, strlen(infostr4) + strlen(target) + 1, target);
    infoTransform(infostr4);
    printf("can't find %s\n", target);
    CloseHandle(existFile);
  } else {
    CloseHandle(existFile);
    isSuccess = MoveFileA(source, target);
    if (!isSuccess) infoTransform(infostr2);
  }
}

//获取当前路径
// pwd √
void getCurDirectory() {
  DWORD length;
  TCHAR* path = NULL;

  length = GetCurrentDirectory(0, NULL);
  // path = new TCHAR[mySize];
  path = malloc(length * sizeof(TCHAR));
  GetCurrentDirectory(length, (LPWSTR)path);
  printf("%ls\n", path);
}

//获取当前时间
// date √
void getTime() {
  SYSTEMTIME sys;
  GetLocalTime(&sys);
  printf("%4d/%02d/%02d %02d:%02d:%02d.%03d 周%1d\n", sys.wYear, sys.wMonth,
         sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds,
         sys.wDayOfWeek);
}

//显示当前所有进程列表
// ps √
void processShow() {
  int ProcessNum = 0;
  PROCESSENTRY32 currentProcess;
  currentProcess.dwSize = sizeof(currentProcess);
  HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hProcess == INVALID_HANDLE_VALUE) {
    printf("process show failed");
  } else {
    int bMore = Process32First(hProcess, &currentProcess);
    while (bMore) {
      printf("PID=%5u   PName = %30ls \n", currentProcess.th32ProcessID,
             currentProcess.szExeFile);
      bMore = Process32Next(hProcess, &currentProcess);
      ProcessNum++;
    }
    CloseHandle(hProcess);
    printf("---------- there are %d processes running ----------", ProcessNum);
  }
}

//显示当前对应环境变量的值
// enviable variable √
void environShow(const char* target) {
  char* infostr1 = "can't find variable";
  wchar_t buffer[MAX_PATH];
  int isSuccess;
  isSuccess = GetEnvironmentVariableA(target, &buffer, MAX_PATH);
  if (!isSuccess)
    printf("can't find variable %s", target);
  else {
    char* infostr2 =
        malloc(strlen(target) + strlen(" : ") + strlen(buffer) + 1);
    strcpy_s(infostr2, strlen(target) + 1, target);
    strcat_s(infostr2, strlen(infostr2) + 4, " : ");
    strcat_s(infostr2, strlen(infostr2) + strlen(buffer) + 1, buffer);
    infoTransform(infostr2);
  }
}

//  新建|设置环境变量
//  (注意，该命令对环境变量并没有直接影响，只要shell一结束，新建的变量立刻销毁)
// set variable value √
void setenviable(const char* target, const char* value) {
  char* infostr1 = "variable set failed";
  int isSuccess;
  // char* lpname = "shellTest";
  // char* value = "123";
  isSuccess = SetEnvironmentVariable(target, value);
  if (!isSuccess) {
    infoTransform(infostr1);
    printf("variable set failed");
  }
}

// 设置当前工作路径
// cd target √
void setcurrentdirectory(const char* target) {
  printf("target:%s\n", target);
  SetCurrentDirectoryA(target);
  CURDIR = target;
}

// 显示消息，将命令回显
// echo str √
void echo(const char* str) {
  infoTransform(str);
  printf("%s\n", str);
}

// 清除屏幕
// cls
void cls() { system("cls"); }

void pauseP() { system("pause"); }

// shell帮助界面
// help
void helprint() {
  printf("shell帮助，输入help获取本帮助\n");
  char* rule1 = "mkdir";
  char* rule2 = "touch";
  char* rule3 = "cat";
  char* rule4 = "rm -f";
  char* rule5 = "rmdir";
  char* rule6 = "cp -f";
  char* rule7 = "mv";
  char* rule8 = "rename -f";
  char* rule9 = "pwd";
  char* rule10 = "date";
  char* rule11 = "ps";
  char* rule12 = "enviable";
  char* rule13 = "set -e";
  char* rule14 = "find -r";
  char* rule16 = "find -t";
  char* rule17 = "find -k";
  char* rule15 = "echo";
  char* rule18 = "cls";
  char* rule19 = "pause";
  printf("%-10s 创建新目录\n", rule1);
  printf("%-10s 创建新文件\n", rule2);
  printf("%-10s 读取文件内容\n", rule3);
  printf("%-10s 删除文件\n", rule4);
  printf("%-10s 删除空目录\n", rule5);
  printf("%-10s 复制文件\n", rule6);
  printf("%-10s 移动文件\n", rule7);
  printf("%-10s 重命名文件\n", rule8);
  printf("%-10s 显示当前路径\n", rule9);
  printf("%-10s 显示当前日期时间\n", rule10);
  printf("%-10s 显示进程列表\n", rule11);
  printf("%-10s 查询环境变量\n", rule12);
  printf("%-10s 修改/新建环境变量\n", rule13);
  printf("%-10s 在当前目录中根据正则表达式查找文件\n", rule14);
  printf("%-10s 在当前目录中查找指定类型的文件\n", rule16);
  printf("%-10s 在当前目录中根据关键字查找文件\n", rule17);
  printf("%-10s 显示消息，将命令回显\n", rule15);
  printf("%-10s 清除屏幕\n", rule18);
  printf("%-10s 暂停\n", rule19);
}
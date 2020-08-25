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
    case cm_if:
      exec_if(cmd);
    case cm_while:
      exec_while(cmd);
    case cm_until:
      exec_until(cmd);
    case cm_for:
      exec_until(cmd);
    case cm_simple:
      exec_simple(cmd);
    default:
      exit(-1);
  }
}

void exec_assign(COMMAND* cmd) {
  ASSIGN_COM* assign = cmd->value.Assign;
  Var* new = (Var*)malloc(sizeof(Var));
  new->name = assign->name;
  new->value = assign->value;
  new->type = assign->type;
  var_insert(assign->name, new);
}

void exec_if(COMMAND* cmd) {
  IF_COM* if_com = cmd->value.If;
  COMMAND* c = NULL;
  if (cal(if_com->test)) {
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
  while (cal(while_com->test)) {
    c = while_com->action;
    while (c != NULL) {
      exec_command(c);
      c = c->next;
    }
  }
}

void exec_until(COMMAND* cmd) {
  UNTIL_COM* until_com = cmd->value.Until;
  COMMAND* c = until_com->action;
  do {
    while (c != NULL) {
      exec_command(c);
      c = c->next;
    }
  } while (cal(until_com->test));
}

void exec_for(COMMAND* cmd) { 
    FOR_COM* for_com = cmd->value.For; 
    for (int i = 0; i < for_com->len_list; ++i) {
      Var* v = val_get(for_com->var);
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
	printf("simple_com->name:%s\n", simple_com->name);
  if (strcmp(simple_com->name, "echo")) {
      printf("%s\n", (simple_com->in)[0]);
  }
  else if (strcmp(simple_com->name, "mkdir"))
  {
	  if (strcmp((simple_com->in)[0], "-f"))
	  {
		  char *target = (simple_com->in)[1];
		  createFile(target);
	  }
	  else {
		  char *target = (simple_com->in)[0];
		  createDirectory(target);
	  }
  }
  else if (strcmp(simple_com->name, "pwd"))
  {
	  getCurDirectory();
  }
  else if (strcmp(simple_com->name, "read"))
  {
	  char *target = (simple_com->in)[0];
	  readFile(target);
  }
  else if (strcmp(simple_com->name, "rm"))
  {
	  if (strcmp((simple_com->in)[0], "-f"))
	  {
		  char *target = (simple_com->in)[1];
		  deleteFile(target);
	  }
	  else {
		  printf("��ָ����δ����");
	  }
  }
  else if (strcmp(simple_com->name, "find"))
  {
	  if (strcmp((simple_com->in)[0], "-r"))
	  {
		  char *regex = (simple_com->in)[1];
		  findfile(regex);
	  }
	  else if (strcmp((simple_com->in)[0], "-t"))
	  {
		  char *regex = (simple_com->in)[1];
		  findfile3(regex);
	  }
	  else if (strcmp((simple_com->in)[0], "-k"))
	  {
		  char *regex = (simple_com->in)[1];
		  findfile2(regex);
	  }
	  else {
		  printf("����ָ����δ����");
	  }
  }
  else if (strcmp(simple_com->name, "ls"))
  {
	  curdirlist();
  }
  else if (strcmp(simple_com->name, "rmdir"))
  {
	  char *target = (simple_com->in)[0];
	  removeDirectory(target);
  }
  else if (strcmp(simple_com->name, "cp"))
  {
	  if()
  }
}




char* infoTransform(char *str) {
	printf("%s\n",str);
	return str;
}



//������Ŀ¼
// mkdir directory
void createDirectory(const char* target)
{
	char * infostr1 = "create failed";
	int isSuccess;
	isSuccess = CreateDirectory(target, NULL);
	if (!isSuccess)
		infoTransform(infostr1);
}


//�������ļ�
// mkdir -f filename
void createFile(const char* target)
{
	char * infostr1 = "file already existed, create failed\n";
	char * infostr2 = "create failed\n";
	char * infostr3 = "create success\n";
	int isSuccess;
	HANDLE existFile = CreateFile(target, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (existFile != INVALID_HANDLE_VALUE)
	{
		infoTransform(infostr1);
	}
	else
	{
		HANDLE newFile = CreateFile(target, GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (newFile == INVALID_HANDLE_VALUE)
			infoTransform(infostr2);
		else infoTransform(infostr3);
		CloseHandle(newFile);
	}

	CloseHandle(existFile);

}


//��ȡ�ļ�����
// read filename
void readFile(const char* target)
{
	char * infostr1 = "file not existed/file open failed\n";
	char * infostr2 = "readfile failed\n";
	char * infostr3 = "file detail:\n";

	int isSuccess;
	HANDLE newFile = CreateFile(target, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (newFile == INVALID_HANDLE_VALUE)
	{
		infoTransform(infostr1);
	}
	else {
		int file_size = 0;
		file_size = GetFileSize(newFile, NULL);
		char * buff;
		buff = (char*)malloc(file_size);
		DWORD dwRead;
		if (!ReadFile(newFile, buff, file_size, &dwRead, NULL))
		{
			infoTransform(infostr2);
		}
		buff[file_size] = '\0';
		char * infostr4 = malloc(strlen(infostr3) + strlen(buff) + 1);
		strcpy(infostr4, infostr3);
		strcat(infostr4, buff);
		infoTransform(infostr4);
		printf("file detail:\n%s\n", buff);
		CloseHandle(newFile);
	}
}

//ɾ���ļ�
// rm -f filename
void deleteFile(const char* target)
{
	char * infostr1 = "can't find ";
	char * infostr2 = "delete failed, the file might be using or need authority ";
	int isSuccess;
	HANDLE existFile = CreateFile(target, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (existFile == INVALID_HANDLE_VALUE)
	{
		char * infostr4 = malloc(strlen(infostr1) + strlen(target) + 1);
		strcpy(infostr4, infostr1);
		strcat(infostr4, target);
		infoTransform(infostr4);

		printf("can't find %s\n", target);
		CloseHandle(existFile);
	}

	else {
		CloseHandle(existFile);
		isSuccess = DeleteFile(target);
		if (!isSuccess)
			infoTransform(infostr2);
	}
}



//�ڵ�ǰĿ¼�¸���������ʽ�����ļ���Ŀ¼+�ļ���
// find -r regex


void findfile(const char* regex) {
	struct _finddata_t data;
	long hnd = _findfirst(regex, &data);
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

//�ڵ�ǰĿ¼�²�����Ӧ���͵��ļ���Ŀ¼+�ļ���
// find -t type

void findfile3(const char* type) {
	struct _finddata_t data;
	char * regex3 = "*.";
	char * regex4 = malloc(strlen(regex3) + strlen(type) + 1);
	strcpy(regex4, regex3);
	strcat(regex4, type);
	//printf("regex4: %s\n",regex4);
	long hnd = _findfirst(regex4, &data);
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


//�ڵ�ǰĿ¼�¸��ݹؼ��ֲ����ļ���Ŀ¼+�ļ���
// find -k key
void findfile2(char* key) {
	struct _finddata_t data;
	char * regex2 = "*";
	char * regex5 = "*.*";
	//printf("sizeof(regex2):%d\n", sizeof(regex2));
	char * regex4 = malloc(strlen(regex2) + strlen(key) + strlen(regex5) + 1);
	strcpy(regex4, regex2);
	strcat(regex4, key);
	strcat(regex4, regex5);
	// strcat_s(regex2,16, key);
	 //strcat_s(regex2, 16, "*.*");


	 //printf("regex4:%s\n", regex4);

	long hnd = _findfirst(regex4, &data);
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




//�г���ǰĿ¼�µ������ļ���Ŀ¼+�ļ���
// ls 
void curdirlist() {
	struct _finddata_t data;
	long hnd = _findfirst("*", &data);
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
��ʱ�ϳ�
//�г�ָ��Ŀ¼�µ������ļ���Ŀ¼+�ļ���
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




//ɾ��Ŀ¼�����ҽ���Ŀ¼Ϊ�գ�
//rmdir directory
void removeDirectory(const char* target)
{
	char * infostr1 = "delete failed, the directory might not empty";
	int isSuccess;
	isSuccess = RemoveDirectory(target);
	if (!isSuccess)
		infoTransform(infostr1);
}



//�����ļ�
// cp -f sourcefile targetfile
void fileCopy(const char* source, const char* target)
{
	char * infostr1 = "source path and target path can't be same";
	char * infostr2 = "copy failed";
	int isSuccess;
	int whetherTwo = 0;
	if (source == target)
		infoTransform(infostr1);
	else {
		isSuccess = CopyFile(source, target, whetherTwo);
		if (!isSuccess)
			infoTransform(infostr2);
	}
}


//�ƶ��ļ�
//mv  sourcefile targetfile
void movefile(const char* source, const char* target)
{
	char * infostr1 = "move failed\n";
	int isSuccess;
	if (source == target)
		return;
	else {
		isSuccess = MoveFile(source, target);
		if (!isSuccess)
			infoTransform(infostr1);
	}
}

//�������ļ�
//rename -f preName LaterName
void fileReName(const char* source, const char* target)
{
	char * infostr1 = "can't find ";
	char * infostr2 = "rename failed";
	int isSuccess;
	HANDLE existFile = CreateFile(source, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (existFile == INVALID_HANDLE_VALUE)
	{
		char * infostr4 = malloc(strlen(infostr1) + strlen(target) + 1);
		strcpy(infostr4, infostr1);
		strcat(infostr4, target);
		infoTransform(infostr4);
		printf("can't find %s\n", target);
		CloseHandle(existFile);
	}
	else {
		CloseHandle(existFile);
		isSuccess = MoveFile(source, target);
		if (!isSuccess)
			infoTransform(infostr2);
	}
}

//��ȡ��ǰ·��
//pwd
void getCurDirectory()
{
	DWORD length;
	TCHAR* path = NULL;

	length = GetCurrentDirectory(0, NULL);
	//path = new TCHAR[mySize];
	path = malloc(length * sizeof(TCHAR));
	GetCurrentDirectory(length, (LPWSTR)path);
	infoTransform(path);
	printf("%s \n", path);
}

//��ȡ��ǰʱ��
//date
void getTime() {
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	printf("%4d/%02d/%02d %02d:%02d:%02d.%03d ��%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);
}


//��ʾ��ǰ���н����б�
//ps
void processShow() {
	int ProcessNum = 0;
	PROCESSENTRY32 currentProcess;
	currentProcess.dwSize = sizeof(currentProcess);
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		printf("process show failed");
	}
	else {
		int bMore = Process32First(hProcess, &currentProcess);
		while (bMore) {
			printf("PID=%5u   PName = %30s \n", currentProcess.th32ProcessID, currentProcess.szExeFile);
			bMore = Process32Next(hProcess, &currentProcess);
			ProcessNum++;
		}
		CloseHandle(hProcess);
		printf("---------- there are %d processes running ----------", ProcessNum);

	}
}

//��ʾ��ǰ��Ӧ����������ֵ  
//enviable variable
void environShow(const char* target) {
	char * infostr1 = "can't find variable";
	char * buffer = NULL;
	int isSuccess; 
	isSuccess = GetEnvironmentVariable(target, &buffer, MAX_PATH);
	if (!isSuccess)
		printf("can't find variable %s", target);
	else
	{
		char * infostr2 = malloc(strlen(target) + strlen(":") + strlen(buffer) + 1);
		strcpy(infostr2, target);
		strcat(infostr2, ":");
		strcat(infostr2, buffer);
		infoTransform(infostr2);
		printf("%s : %s", target, buffer);
	}
}

//  �½�|���û������� (ע�⣬������Ի���������û��ֱ��Ӱ�죬ֻҪshellһ�������½��ı�����������) 
//set variable value
void setenviable(const char* target, const char* value)
{
	char * infostr1 = "variable set failed";
	int isSuccess;
	//char* lpname = "shellTest";
	//char* value = "123";
	isSuccess = SetEnvironmentVariable(target, value);
	if (!isSuccess)
	{
		infoTransform(infostr1);
		printf("variable set failed");
	}
}

// ���õ�ǰ����·��  
//cd target
void setcurrentdirectory(const char* target)
{
	SetCurrentDirectory(target);
	CURDIR = target;
}

// ��ʾ��Ϣ�����������
//echo str
void echo(const char* str)
{
	infoTransform(str);
	printf("%s\n", str);
}

// �����Ļ
//cls
void cls()
{
	system("cls");
}

void pauseP()
{
	system("pause");
}


//shell��������  
//help
void helprint() {
	printf("shell����������help��ȡ������\n");
	char * rule1 = "mkdir";
	char * rule2 = "mkdir -f";
	char * rule3 = "read";
	char * rule4 = "rm -f";
	char * rule5 = "rmdir";
	char * rule6 = "cp -f";
	char * rule7 = "mv";
	char * rule8 = "rename -f";
	char * rule9 = "pwd";
	char * rule10 = "date";
	char * rule11 = "ps";
	char * rule12 = "enviable";
	char * rule13 = "set -e";
	char  *rule14 = "find -r";
	char  *rule16 = "find -t";
	char  *rule17 = "find -k";
	char  *rule15 = "echo";
	char *rule18 = "cls";
	char *rule19 = "pause";
	printf("%-10s ������Ŀ¼\n", rule1);
	printf("%-10s �������ļ�\n", rule2);
	printf("%-10s ��ȡ�ļ�����\n", rule3);
	printf("%-10s ɾ���ļ�\n", rule4);
	printf("%-10s ɾ����Ŀ¼\n", rule5);
	printf("%-10s �����ļ�\n", rule6);
	printf("%-10s �ƶ��ļ�\n", rule7);
	printf("%-10s �������ļ�\n", rule8);
	printf("%-10s ��ʾ��ǰ·��\n", rule9);
	printf("%-10s ��ʾ��ǰ����ʱ��\n", rule10);
	printf("%-10s ��ʾ�����б�\n", rule11);
	printf("%-10s ��ѯ��������\n", rule12);
	printf("%-10s �޸�/�½���������\n", rule13);
	printf("%-10s �ڵ�ǰĿ¼�и���������ʽ�����ļ�\n", rule14);
	printf("%-10s �ڵ�ǰĿ¼�в���ָ�����͵��ļ�\n", rule16);
	printf("%-10s �ڵ�ǰĿ¼�и��ݹؼ��ֲ����ļ�\n", rule17);
	printf("%-10s ��ʾ��Ϣ�����������\n", rule15);
	printf("%-10s �����Ļ\n", rule18);
	printf("%-10s ��ͣ\n", rule19);
}


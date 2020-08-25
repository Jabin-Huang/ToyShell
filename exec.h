#pragma once

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<windows.h>
#include<io.h>
#include <tlhelp32.h>

#include"command.h"

void  createDirectory(const char*);
void createFile(const char*);
void readFile(const char*);
void deleteFile(const char*);
void dirlist(const char*);
void removeDirectory(const char*);
void fileCopy(const char*, const char*);
void fileCopy(const char*, const char*);
void movefile(const char*, const char*);
void fileReName(const char*, const char*);
void getCurDirectory();
void getTime();
void helprint();
void processShow();
void environShow(const char*);
void setenviable(const char*, const char*);
void curdirlist();
void setcurrentdirectory(const char*);
void findfile(const char*);
void findfile2(char*);
void findfile3(const char*);
void echo(const char*);
void cls();
void pauseP();
char* infoTransform(char *);

#define NEW_DIRECTORY "F:\\OSClassDesign\\testDirectory\\test1"
#define TARGET_FILE "F:\\OSClassDesign\\testDirectory\\test1\\2.txt"
#define SOURCE "F:\\OSClassDesign\\testDirectory\\test1\\scscxt"
#define TARGET "F:\\OSClassDesign\\testDirectory\\test1\\2.txt\\th.txt"

char * CURDIR = "F:\\VS data\\default save\\Repos\\Project1\\Project1";


void exec_command(COMMAND* cmd);

void exec_assign(COMMAND* cmd);

void exec_if(COMMAND* cmd);

void exec_while(COMMAND* cmd);

void exec_until(COMMAND* cmd);

void exec_for(COMMAND* cmd);

void exec_simple(COMMAND* cmd);

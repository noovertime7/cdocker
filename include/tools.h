#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h> // 包含 exit() 的声明
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// 自定义错误处理函数
void sys_err(const char* msg);

// 设置容器主机名的函数
void set_container_hostname(const char* hostname);

void write_file(const char *filename, const char *content);

#endif // TOOLS_H
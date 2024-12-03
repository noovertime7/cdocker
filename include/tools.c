
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "tools.h"
#include "common.h"

// 自定义错误处理函数
void sys_err(const char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE); // 使用 EXIT_FAILURE 代替直接写数字
}

void set_container_hostname(const char *hostname)
{
  if (sethostname(hostname, strlen(hostname)) == -1)
  {
    sys_err("set hostname error");
  }
}

void write_file(const char *filename, const char *content)
{
  int fd = open(filename, O_WRONLY);

  if (-1 == fd)
  {
    perror("open");
    printf("    %s: %s\n", filename, content);
    exit(EXIT_FAILURE);
  }

  if (-1 == write(fd, content, strlen(content)))
  {
    perror("write");
    printf("    %s: %s\n", filename, content);
    exit(EXIT_FAILURE);
  }

  close(fd);
}

void ensure_directory_exists(const char *path)
{
  struct stat info;

  if (stat(path, &info) != 0)
  {
    if (errno == ENOENT)
    {
      if (mkdir(path, 0755) == 0)
      {
        printf("create dir success: %s\n", path);
        return;
      }
      else
      {
        perror("目录创建失败");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      perror("stat 错误");
      exit(EXIT_FAILURE);
    }
  }
  else if (info.st_mode & S_IFDIR)
  {
    printf("dir exist: %s\n", path);
    return;
  }
  else
  {
    fprintf(stderr, "路径存在但不是目录: %s\n", path);
  }
}

#include "container.h"
#include "tools.h"
#include "overlayfs.h"
#include "namespace.h"

#define HOSTNAME "cdocker"

char *merged = "rootfs/merged";

void setup_container()
{
  printf("Setting up container environment...\n");

  if (0 != chdir(merged))
  {
    perror("chdir");
    return;
  }
  if (0 != chroot("."))
  {
    perror("chroot");
    return;
  }

  if (mount("proc", "/proc", "proc", 0, NULL) == -1)
  {
    perror("mount proc");
    exit(EXIT_FAILURE);
  }
}

void run_container_process()
{
  // 在子进程中运行容器命令
  char *const args[] = {"/bin/bash", NULL};
  execv("/bin/bash", args);

  // 如果 execv 返回，说明出错了
  perror("execv");

  exit(EXIT_FAILURE);
}

int container_run()
{
  pid_t child_pid = fork();
  if (-1 == child_pid)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (0 == child_pid)
  {
    printf("Parent (PID: %d) created a child (PID: %d)..\n", getppid(), getpid());

    // 需要在unshare之前运行
    {
      char *lowerdir = "rootfs/lowdir";
      char *upperdir = "rootfs/upperdir";
      char *workdir = "rootfs/workdir";
      char *procdir = "rootfs/lowdir/proc";

      ensure_directory_exists(lowerdir);
      ensure_directory_exists(upperdir);
      ensure_directory_exists(workdir);
      ensure_directory_exists(procdir);
      ensure_directory_exists(merged);

      // setup_overlayfs(lowerdir, upperdir, workdir, merged);
    }

    if (-1 == unshare(CLONE_NEWUTS | CLONE_NEWUSER | CLONE_NEWNET | CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWIPC))
    {
      perror("unshare");
      exit(EXIT_FAILURE);
    }

    printf("after unshare, pid: %d\n", getpid());

    {
      // 初始化容器
      set_container_hostname(HOSTNAME);

      setup_user_namespace(getpid());

      // set_environment_variable("PATH=/bin");
    }

    {
      // 再次 fork 一个子进程来运行容器进程
      pid_t container_pid = fork();
      if (-1 == container_pid)
      {
        perror("fork");
        exit(EXIT_FAILURE);
      }

      if (0 == container_pid)
      {
        printf("enter container..\n");

        setup_container();

        run_container_process();
      }
      else
      {
        // 等待容器进程结束
        waitpid(container_pid, NULL, 0);

        printf("container process (PID: %d) exit..\n", container_pid);
      }
    }

    exit(EXIT_SUCCESS);
  }
  else
  {
    // 父进程
    waitpid(child_pid, NULL, 0);

    if (-1 == umount(merged))
    {
      perror("umount overlay");
    }
    else
    {
      printf("umount %s success..\n", merged);
    }

    printf("child (PID: %d) exit..\n", child_pid);
  }

  return EXIT_SUCCESS;
}
#include "container.h"
#include "tools.h"
#include <sched.h>
#include <stdio.h>
#include "namespace.h"
#include <errno.h>
#include <string.h>
#include <unistd.h> // 包含 execv 和其他相关函数的声明



// 声明 run_container_process 函数
void run_container_process(void);

// container_run 函数实现
void container_run() {
    // 调用 unshare 创建一个新的 UTS 命名空间
    if (-1 == unshare(CLONE_NEWUTS |CLONE_NEWUSER )) {
        sys_err("unshare error");
    }

    printf("创建USERNS成功,PID:%d\n",getpid());

    set_container_hostname("cdocker");



    setup_user_namespace(getpid());

    // 运行容器进程
    run_container_process();
}

// 运行容器进程的函数
void run_container_process(void) {
    char* const args[] = {"/bin/bash", NULL};
    execv("/bin/bash", args);

    // 注意：如果 execv 成功，它不会返回到这里。
    // 如果我们确实返回了，那说明 execv 失败了。

    sys_err("exec error");
}

#include <stdio.h>
#include <fcntl.h>
#include "namespace.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tools.h"

void setup_user_namespace(pid_t pid) {
    char path[256];

    printf("Setting up user namespace...\n");

    sprintf(path, "/proc/%d/setgroups", pid);
    write_file(path, "deny\n");

    sprintf(path, "/proc/%d/uid_map", pid);
    write_file(path, "0 0 1\n");

    sprintf(path, "/proc/%d/gid_map", pid);
    write_file(path, "0 0 1\n");
}



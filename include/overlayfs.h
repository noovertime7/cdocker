#ifndef OVERLAYFS_H
#define OVERLAYFS_H

#include "common.h"

void setup_overlayfs(char *lowerdir, char *upperdir, char *workdir, char *mergedir);

#endif // OVERLAYFS_H
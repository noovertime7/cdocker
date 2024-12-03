#include "overlayfs.h"
#include "tools.h"

void setup_overlayfs(char *lowerdir, char *upperdir, char *workdir, char *mergedir)
{
  printf("Setting up overlayfs...\n");

  char mount_data[0x2000];
  memset(mount_data, 0, sizeof(mount_data));

  snprintf(mount_data, sizeof(mount_data), "lowerdir=%s,upperdir=%s,workdir=%s", lowerdir, upperdir, workdir);

  printf("overlay info: %s\n", mount_data);

  if (-1 == mount("overlay", mergedir, "overlay", 0, mount_data))
  {
    perror("mount overlay");
    exit(EXIT_FAILURE);
  }
}
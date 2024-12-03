#include "overlayfs.h"
#include "tools.h"
#include "common.h"

void setup_overlayfs(char *lowerdir, char *upperdir, char *workdir, char *mergedir)
{
  INFO_PRINT("Setting up overlayfs...\n");

  char mount_data[0x2000];
  memset(mount_data, 0, sizeof(mount_data));

  snprintf(mount_data, sizeof(mount_data), "lowerdir=%s,upperdir=%s,workdir=%s", lowerdir, upperdir, workdir);

  INFO_PRINT("overlay info: %s", mount_data);

  if (mount("overlay", mergedir, "overlay", 0, mount_data) == -1)
  {
    perror("mount overlay error");
    exit(EXIT_FAILURE);
  }
}
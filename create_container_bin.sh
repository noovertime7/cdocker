#!/bin/bash
# ./create_container_bin.sh shell命令
# example：./create_container_bin.sh bash

# 检查是否提供了命令参数
if [ -z "$1" ]; then
  echo "Usage: $0 <command>"
  exit 1
fi

# 获取命令的绝对路径
command_path=$(which $1)

# 检查命令是否存在
if [ ! -f "$command_path" ]; then
  echo "Command $1 not found"
  exit 1
fi

# 创建目标目录
lowdir_target_dir="rootfs/lowdir"
bindir_target_dir="rootfs/lowdir/bin"
mkdir -p $lowdir_target_dir
mkdir -p $bindir_target_dir

# 获取命令的依赖库并复制到目标目录
ldd $command_path | egrep -o '/lib[^\ ]*' | while read lib; do
  # 检查库文件是否存在，防止某些库文件可能丢失或不存在
  if [ -f "$lib" ]; then
    cp --parents $lib $lowdir_target_dir
  else
    echo "Library $lib not found, skipping."
  fi
done

# 复制命令到rootfs/bin目录
cp $command_path $bindir_target_dir

echo "All dependencies copied to $lowdir_target_dir"
echo "Command $1 copied to $bindir_target_dir"


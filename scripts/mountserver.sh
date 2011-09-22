#!/bin/bash
mount -t cifs //<ip>/samples /mnt/server/ -o user=$1,noauto,umask=000,user,uid=1001,gid=1001

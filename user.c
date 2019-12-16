#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fls.h"

int main(int argc,char *argv[]){
    int fd;
    int type = 0, arg = 0;

    if (argc != 3) {
        printf("usage user type pid\n");
        return -1;
    }

    type = atoi(argv[1]);
    if (type != FLS_TYPE_PROCESS ||
        type != FLS_TYPE_THREAD) {
        printf("only support fls type is %d or %d",
               FLS_TYPE_PROCESS,
               FLS_TYPE_THREAD);
            return -1;
    }

    arg = atoi(argv[2]);

    //因为驱动直接在/dev/目录下生成了设备节点misc ，因此应用程序可以直接打开该文件。
    fd = open("/dev/fls",O_RDWR);

    if(fd<0){
        printf("device open failed\n");
        return 0;
    }

    ioctl(fd, type, arg);
    return 0;
}
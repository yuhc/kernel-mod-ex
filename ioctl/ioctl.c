/*
 * The process to use IOCTL's to control kernel module (instead of cat)
 */

#include "chardev.h"

#include <stdio.h>
#include <stdlib.h>       /* exit */
#include <fcntl.h>        /* open */
#include <unistd.h>       /* close */
#include <sys/ioctl.h>    /* ioctl */

void ioctl_set_msg(int file_desc, char *msg) {
    int ret;

    ret = ioctl(file_desc, IOCTL_SET_MSG, msg);
    if (ret < 0) {
        printf("ioctl_set_msg failed: %d\n", ret);
        exit(-1);
    }
}

void ioctl_get_msg(int file_desc) {
    int ret;
    char msg[100];

    /*
     * In production we would use two IOCTLs - one to tell the kernel
     * the buffer length, and another to give it the buffer to fill
     */
    ret = ioctl(file_desc, IOCTL_GET_MSG, msg);
    if (ret < 0) {
        printf("ioctl_get_msg failed: %d\n", ret);
        exit(-1);
    }

    printf("get_msg: %s\n", msg);
}

void ioctl_get_nth_byte(int file_desc) {
    int i;
    char c;

    printf("get_nth_byte msg: ");
    i = 0;
    do {
        c = ioctl(file_desc, IOCTL_GET_NTH_BYTE, i++);
        if (c < 0) {
            printf("ioctl_get_nth_byte failed at %d'th byte\n", i);
            exit(-1);
        }
        putchar(c);
    } while (c != 0);
    putchar('\n');
}

int main() {
    int file_desc, ret;
    char *msg = "Message passed by IOCTL\n";

    file_desc = open(DEVICE_FILE_NAME, 0);
    ioctl_get_nth_byte(file_desc);
    ioctl_get_msg(file_desc);
    ioctl_set_msg(file_desc, msg);
    ioctl_get_msg(file_desc);

    close(file_desc);

    return 0;
}

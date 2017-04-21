#ifndef __CHAR_DEV_H__
#define __CHAR_DEV_H__

#include <asm/ioctl.h>

/*
 * Static major device number
 */
#define MAJOR_NUM 100

/*
 * Create a IOCTL command number for passing information from a user
 * process to the kernel module
 */
#define IOCTL_SET_MSG       _IOR(MAJOR_NUM, 0, char *)

/*
 * Get the message of device driver
 */
#define IOCTL_GET_MSG       _IOR(MAJOR_NUM, 1, char *)
#define IOCTL_GET_NTH_BYTE  _IOR(MAJOR_NUM, 2, int)

/*
 * Device name
 */
#define DEVICE_FILE_NAME    "helloworld"

#endif

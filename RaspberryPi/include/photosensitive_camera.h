#ifndef __PHOTOSENSITIVE_CAMERA_H__
#define __PHOTOSENSITIVE_CAMERA_H__

#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/wait.h>

/*光敏模块的数字逻辑输出端DO接树莓派21号引脚*/
#define DO 21

int waitForMyPic(void);

#endif


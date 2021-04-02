#ifndef __PHOTOSENSITIVE_CAMERA_H__
#define __PHOTOSENSITIVE_CAMERA_H__

#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/wait.h>

/*Digtal out from photosensitive sensor*/
#define DO 2

int waitForMyPic(void);

#endif


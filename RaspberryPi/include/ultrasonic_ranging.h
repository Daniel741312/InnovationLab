#ifndef __ULTRASONIC_RANGING_H__
#define __ULTRASONIC_RANGING_H__

#include <sys/time.h>
#include <wiringPi.h>

#include <iostream>

int ultraInit(void);
float disMeasure(void);

#endif

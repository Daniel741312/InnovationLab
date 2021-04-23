#ifndef __ULTRASONIC_RANGING_H__
#define __ULTRASONIC_RANGING_H__

#include <wiringPi.h>
#include <iostream>
#include <sys/time.h>

int ultraInit(void);
float disMeasure(void);

#endif

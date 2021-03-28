#ifndef __ULTRASONIC_RANGING_H__
#define __ULTRASONIC_RANGING_H__

#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

void ultraInit(void);
float disMeasure(void);

#endif
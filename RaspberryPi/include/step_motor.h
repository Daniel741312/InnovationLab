#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include <wiringPi.h>
#include <iostream>

#define IN1 22
#define IN2 23
#define IN3 24
#define IN4 25

/*控制步进电机转速的*/
#define	DELAY 4

/*宏函数，依次向四个引脚写入a,b,c,d四个值*/
#define SETSTEP(a,b,c,d)\
	digitalWrite(IN1,a);\
	digitalWrite(IN2,b);\
	digitalWrite(IN3,c);\
	digitalWrite(IN4,d);\

void stop(void);
void forward(int steps);
void backward(int steps);
int motorInit(void);

#endif

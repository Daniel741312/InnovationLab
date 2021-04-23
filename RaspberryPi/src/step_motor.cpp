#include "step_motor.h"

/*原来的那个函数，可能会有用*/
#if 0 
static void setStep(int a, int b, int c, int d){
	digitalWrite(IN1, a);	
	digitalWrite(IN2, b);	
	digitalWrite(IN3, c);	
	digitalWrite(IN4, d);	
	return;
}
#endif

void stop(void){
	SETSTEP(0, 0, 0, 0);	
	return;
}

void forward(int steps){
	for(int i = 0; i < steps; i++){
		SETSTEP(1, 0, 0, 0);
		delay(DELAY);
		SETSTEP(0, 1, 0, 0);	
		delay(DELAY);
		SETSTEP(0, 0, 1, 0);	
		delay(DELAY);
		SETSTEP(0, 0, 0, 1);	
		delay(DELAY);
	}
	return;
}

void backward(int steps){
	for(int i = 0; i < steps; i++){
		SETSTEP(0, 0, 0, 1);
		delay(DELAY);
		SETSTEP(0, 0, 1, 0);	
		delay(DELAY);
		SETSTEP(0, 1, 0, 0);	
		delay(DELAY);
		SETSTEP(1, 0, 0, 0);	
		delay(DELAY);
	}
	return;
}

int motorInit(void){
	if (-1 == wiringPiSetup()) {
		std::cerr<<"Setup wiringPi failed"<<std::endl;
		return -1;
	}
	/*设置四个引脚为输出模式*/
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	return 0;
}

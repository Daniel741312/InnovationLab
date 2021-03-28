#include "../include/ultrasonic_ranging.h"

#define Trig    0  
#define Echo    1  

/*设置0号引脚输出触发信号，1号引脚输入返回的信号，注意这里的引脚是按照wiringPi编码的，参考README中的树莓派引脚图*/
void ultraInit(void){  
	pinMode(Echo, INPUT);  
	pinMode(Trig, OUTPUT);  
	return;
}  

float disMeasure(void){  
	/*linux系统自带的时间结构体*/
	struct timeval tv1;
	struct timeval tv2;
	long start, stop;
	float dis;

	/*Trig写2us的低电平*/
	digitalWrite(Trig, LOW);  
	delayMicroseconds(2);  

	/*Trig写10us的高电平*/
	digitalWrite(Trig, HIGH);  
	delayMicroseconds(10);
	digitalWrite(Trig, LOW);

	/*
		*Trig信号：
		*	|----------|
			|          |
		  --		    ----
	*/

	/*Echo==1的时刻是发射ultrasonic的时刻，只要Echo!=1，就死循环阻塞等待在这儿*/
	while(digitalRead(Echo) != 1);
	/*跳出上面的循环时，就是ultrasonic发射的时刻*/  
	gettimeofday(&tv1, NULL);

	/*同理，tv2是接收到回射超声波的时刻*/
	while(digitalRead(Echo) != 0);  
	gettimeofday(&tv2, NULL);

	start = tv1.tv_sec * 1000000 + tv1.tv_usec;
	stop  = tv2.tv_sec * 1000000 + tv2.tv_usec;  

	dis = (float)(stop - start) / 1000000 * 34000 / 2;

	return dis;  
}  

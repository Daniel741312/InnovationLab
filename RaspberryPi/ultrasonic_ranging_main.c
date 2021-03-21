#include "include/ultrasonic_ranging.h"

int main(int argc,char* argv[]){  
	float dis;  

	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !\n");  
		return 1;   
	}  

	ultraInit();  

	while(1){  
		dis = disMeasure();  
		printf("distance = %0.2f cm\n",dis);  
		delay(1000);
	} 

	return 0;  
} 
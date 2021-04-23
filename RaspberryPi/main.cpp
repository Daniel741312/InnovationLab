#include "tcp_send.h"
#include "ultrasonic_ranging.h"
#include "waste_sorting.h"
#include "photosensitive_camera.h"
#include "step_motor.h"
#include <stdio.h>

/*测试函数*/
static void tcp_send_test(const char* str);
static void ultrasonic_ranging_test();
static void step_motor_test(const std::string orientation,const int steps){

int main(int argc, char* argv[]){
	const char* str = "{\"id\":6,\"recyleBitMap\":14,\"location\":[120.3532, 30.3242],\"usage\":[0.9, 1, 1, 0.5]}";

	int type=0;
	const char* fileName="garbage.jpg";
	std::vector<std::string>* objectNames=new std::vector<std::string>();

	step_motor_test("clockwise",128);

	while(1){
		waitForMyPic();

		getGarbageNamesByImage(fileName,objectNames);
		type=getGarbageCategoryByNames(objectNames);

		(*objectNames).erase((*objectNames).begin(),(*objectNames).end());
#ifdef TEST
		std::cout<<"----In the waste_sorting:type="<<type<<"----\n\n"<<std::endl;
#endif
	}

	tcp_send_test(str);
	ultrasonic_ranging_test();
	step_motor_test("clockwise",128);


	delete(objectNames);
	return 0;
}

static void tcp_send_test(const char* str){
	int ret = sendInfoByTCP(str, strlen(str));
	printf("----tcp_send_test:%d----\n", ret);
	return;
}

static void ultrasonic_ranging_test(){
	float dis;
	int times=30;

	if (wiringPiSetup() == -1){
		printf("setup wiringPi failed !\n");
		return;
	}

	ultraInit();

	while (times--){
		dis = disMeasure();
		printf("distance = %0.2f cm\n", dis);
		delay(1000);
	}

	return;
}

static void step_motor_test(const string orientation,const int steps){
	motorInit();

	if(orientation=="clockwise"){
		forward(steps);
	}else if(orientation=="anticlockwise"){
		backward(steps);
	}else{
		std<<cout<<"wrong arg1"<<endl;
	}

	stop();
	return;
}

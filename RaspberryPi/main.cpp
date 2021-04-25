#include "tcp_send.h"
#include "waste_sorting.h"
#include "photosensitive_camera.h"
#include "ultrasonic_ranging.h"
#include "step_motor.h"
#include "servo_motor.h"
#include <stdio.h>
#include <unistd.h>

/*测试函数*/
static void tcp_send_test(const char* str);
static void ultrasonic_ranging_test();
static void step_motor_test(std::string orientation,int steps);

int throwItAndReport(int steps);

std::string g_jsonStrToBeSent=R"({"id":6,"recyleBitMap":14,"location":[120.3532, 30.3242],"usage":[0.9, 1, 1, 0.5]})";

int main(int argc, char* argv[]){

	int type=0;
	const char* fileName="garbage.jpg";
	std::vector<std::string>* objectNames=new std::vector<std::string>();

	/*一些调试用的代码*/
	if(argc==3){
		/*可以微调步进电机走指定的步数，参数格式如下：./main (anti)clockwise 10*/
		int steps0=atoi(argv[2]);
		motorInit();
		step_motor_test(argv[1],steps0);
		stop();
		delete(objectNames);
		return 0;
	}else if(argc==2){
		/*指定物体名称，这样调试的时候就不用每次都挡住光敏电阻触发拍照了，参数格式如下： ./main 电池*/
		objectNames->push_back(argv[1]);
		type=getGarbageCategoryByNames(objectNames);
		(*objectNames).erase((*objectNames).begin(),(*objectNames).end());
		std::cout<<"----In the waste_sorting:type="<<type<<"----\n\n"<<std::endl;
		switch(type){
			case(0):{
				throwItAndReport(0);
				break;
			}
			case(1):{
				throwItAndReport(128);
				break;
			}
			case(2):{
				throwItAndReport(256);
				break;
			}
			case(3):{
				throwItAndReport(-128);
				break;
			}
			default:
				break;
		}
		delete(objectNames);
		return 0;
	}

	while(1){

		waitForMyPic();
		getGarbageNamesByImage(fileName,objectNames);
		type=getGarbageCategoryByNames(objectNames);
		(*objectNames).erase((*objectNames).begin(),(*objectNames).end());
		std::cout<<"----In the waste_sorting:type="<<type<<"----\n\n"<<std::endl;

		/*
			0-可回收-蓝色
  	 		1-有害-红色
  			2-厨余(湿)-绿色
   			3-其他(干)-黄色
		*/
		switch(type){
			case(0):{
				throwItAndReport(0);
				break;
			}
			case(1):{
				throwItAndReport(128);
				break;
			}
			case(2):{
				throwItAndReport(256);
				break;
			}
			case(3):{
				throwItAndReport(-128);
				break;
			}
			default:
				break;
		}

	}

	delete(objectNames);
	return 0;
}

int throwItAndReport(int steps){
	motorInit();
	if(steps>=0){
		forward(steps);
		stop();
		sleep(1);
		throwGarbage();
		backward(steps);
		stop();
	}else{
		backward(-steps);
		stop();
		sleep(1);
		throwGarbage();
		forward(-steps);
		stop();
	}
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

static void step_motor_test(std::string orientation,int steps){
	motorInit();

	if(orientation=="clockwise"){
		forward(steps);
	}else if(orientation=="anticlockwise"){
		backward(steps);
	}else{
		std::cout<<"wrong arg1"<<std::endl;
	}

	stop();
	return;
}

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

float throwItAndGetTheOppositeUsage(int steps);

std::string g_jsonStrToBeSent=R"({"id":6,"recyleBitMap":14,"location":[120.3532, 30.3242],"usage":[0.9, 1, 1, 0.5]})";

int main(int argc, char* argv[]){

	//ultrasonic_ranging_test();
	int type=0;
	float usage=0;
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
		std::cout<<__func__<<"-type-"<<type<<std::endl;
		switch(type){
			case(0):{
				usage=throwItAndGetTheOppositeUsage(0);
				break;
			}
			case(1):{
				usage=throwItAndGetTheOppositeUsage(128);
				break;
			}
			case(2):{
				usage=throwItAndGetTheOppositeUsage(256);
				break;
			}
			case(3):{
				usage=throwItAndGetTheOppositeUsage(-128);
				break;
			}
			default:
				break;
		}
		delete(objectNames);
		std::cout<<__func__<<"-usage-"<<usage<<std::endl;
		return 0;
	}

	while(1){

		waitForMyPic();
		getGarbageNamesByImage(fileName,objectNames);
		type=getGarbageCategoryByNames(objectNames);
		(*objectNames).erase((*objectNames).begin(),(*objectNames).end());
		std::cout<<__func__<<"-type-"<<type<<std::endl;

		/*
			0-可回收-蓝色-顺时针转0度-steps=0
  	 		1-有害-红色-顺时针转90度-steps=128
  			2-厨余(湿)-绿色-顺时针转180度-steps=256
   			3-其他(干)-黄色-逆时针转90度-steps=-128
		*/
		switch(type){
			case(0):{
				usage=throwItAndGetTheOppositeUsage(0);
				break;
			}
			case(1):{
				usage=throwItAndGetTheOppositeUsage(128);
				break;
			}
			case(2):{
				usage=throwItAndGetTheOppositeUsage(256);
				break;
			}
			case(3):{
				usage=throwItAndGetTheOppositeUsage(-128);
				break;
			}
			default:
				break;
		}

	}

	delete(objectNames);
	return 0;
}

float throwItAndGetTheOppositeUsage(int steps){
	motorInit();
	float dis=0;
	float usage=0;
	if(steps>=0){
		forward(steps);
		stop();
		sleep(1);
		/*超声波测距模块距离桶底的高度为22cm，距离桶顶的高度为8.7cm*/
		dis=disMeasure();
		usage=(22-dis)/13;
		throwGarbage();
		backward(steps);
		stop();
	}else{
		backward(-steps);
		stop();
		dis=disMeasure();
		usage=(22-dis)/13;
		sleep(1);
		throwGarbage();
		forward(-steps);
		stop();
	}
	/*保留一位小数*/
	//usage=(int(usage*10+0.5))/10;
	return usage;
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

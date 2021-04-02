#include "tcp_send.h"
#include "ultrasonic_ranging.h"
#include "waste_sorting.h"
#include "photosensitive_camera.h"

/*现在main函数充当测试的作用吧*/

static void tcp_send_test(const char *str);
static void ultrasonic_ranging_test();
static void waste_sorting_test(const char* fileName);
static void photosensitive_camera_test(void);

int main(int argc, char* argv[]){
#if 0
	int n=argc;
	char** strv=argv;
	waste_sorting_test(n,strv);

	const char* str = "{\"id\":6,\"recyleBitMap\":14,\"location\":[120.3532, 30.3242],\"usage\":[0.9, 1, 1, 0.5]}";
	tcp_send_test(str);

	ultrasonic_ranging_test();
#endif
	while(1){
		waitForMyPic();
		waste_sorting_test("garbage.jpg");
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

static void waste_sorting_test(const char* fileName){

	int type=0;
	std::vector<std::string>* objectNames=new std::vector<std::string>();

	getGarbageNamesByImage(fileName,objectNames);
	type=getGarbageCategoryByNames(objectNames);
	std::cout<<"----In the waste_sorting:type="<<type<<"----\n\n"<<std::endl;

	delete(objectNames);
	return;

}

static void photosensitive_camera_test(void){
	int ret=waitForMyPic();
	if(ret==-1)
		printf("waitFoeMyPic reports an error\n");
	return;
}


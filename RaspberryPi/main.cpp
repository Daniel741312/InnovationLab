#include "tcp_send.h"
#include "ultrasonic_ranging.h"
#include "waste_sorting.h"

/*现在main函数充当测试的作用吧*/

static void tcp_send_test(const char *str);
static void ultrasonic_ranging_test();
static void waste_sorting_test(int argc,char* argv[]);

int main(int argc, char* argv[]){
    int n=argc;
    const char** strv=argv;
    waste_sorting_test(n,strv);

    const char* str = "{\"id\":6,\"recyleBitMap\":14,\"location\":[120.3532, 30.3242],\"usage\":[0.9, 1, 1, 0.5]}";
    tcp_send_test(str);

    ultrasonic_ranging_test();

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

static void waste_sorting_test(int n,const char** strv){

    std::vector<std::string>* objectNames=new std::vector<std::string>();
	int type=0;
	int ret=0;

	for(int i=1;i<n;++i){
		/*从main函数参数中获取图片文件路径*/
		ret=getGarbageNamesByImage(strv[i],objectNames);

		type=getGarbageCategoryByNames(objectNames);
		std::cout<<"----In the waste_sorting:type="<<type<<"----\n\n"<<std::endl;

		(*objectNames).erase((*objectNames).begin(),(*objectNames).end());
	}

	delete(objectNames);
	return;

}

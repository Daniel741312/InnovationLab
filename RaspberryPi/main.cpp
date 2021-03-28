#include "tcp_send.h"
#include "ultrasonic_ranging.h"
#include "waste_sorting.h"

/*现在main函数充当测试的作用吧*/

static void tcp_send_test(const char *str);
static void ultrasonic_ranging_test();
static void waste_sorting_test(int argc,char* argv[]);

int main(int argc, char* argv[]){
    int n=argc;
    char** strv=argv;
    //tcp_send_test("{\"id\":6,\"recyleBitMap\":14,\"location\":[120.3532, 30.3242],\"usage\":[0.9, 1, 1, 0.5]}");
    //ultrasonic_ranging_test();
    waste_sorting_test(n,strv);

    return 0;
}

static void tcp_send_test(const char* str){
    //const char *str = "{\"id\":6,\"recyleBitMap\":14,\"location\":[120.3532, 30.3242],\"usage\":[0.9, 1, 1, 0.5]}";
    int ret = sendInfoByTCP(str, strlen(str));
    printf("----tcp_send_test:%d----\n", ret);
    return;
}

static void ultrasonic_ranging_test(){
    float dis;

    if (wiringPiSetup() == -1){
        printf("setup wiringPi failed !\n");
        return;
    }

    ultraInit();

    while (1){
        dis = disMeasure();
        printf("distance = %0.2f cm\n", dis);
        delay(1000);
    }

    return;
}

static void waste_sorting_test(int argc,char* argv[]){

    std::vector<std::string>* objectNames=new std::vector<std::string>();
	int type=0;
	int ret=0;

	for(int i=1;i<argc;++i){
		/*从main函数参数中获取图片文件路径*/
		ret=getGarbageNamesByImage(argv[i],objectNames);
		if(ret!=0){
			std::cerr<<"BaiduAI reports an error\n"<<std::endl;
			exit(1);
		}

		type=getGarbageCategoryByNames(objectNames);
		std::cout<<"----In the waste_sorting:type="<<type<<"----\n\n"<<std::endl;

		(*objectNames).erase((*objectNames).begin(),(*objectNames).end());
	}

	delete(objectNames);
	return;

}

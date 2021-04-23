#include "photosensitive_camera.h"

/*临时没有光敏电阻模块时，可以用键盘上的a键触发拍照*/
//#define HIT_A_TO_TRIGER

int waitForMyPic(void){
	if(wiringPiSetup()==-1){
		std::cerr<<"Setup wiringPi failed"<<std::endl;
		return -1;
	}

	pinMode(DO,INPUT);

#ifndef HIT_A_TO_TRIGER
	while(1){
		/*光照正常时，引脚为低电平，程序阻塞在这里*/
		while(digitalRead(DO)==0);
		/*类似于消抖，或者说迟滞的作用，跳出循环时，等待0.1秒再去读引脚电平*/
		usleep(1e5);
		/*如果延时后引脚还为高电平，跳出死循环，否则回到死循环*/
		if(digitalRead(DO)==1)
			break;
	}
#else
	while(1){
		char triger='\0';
		/*阻塞等待用户键入a并回车后跳出死循环*/
		scanf("%c",&triger);
		if(triger=='a')
			break;
		else
			continue;
	}
#endif

	/*创建一个子进程，然后让子进程去执行/usr/bin/下的拍照程序raspistill*/
	int forkRet=fork();
	if(forkRet<0){
		perror("fork error");
		return -1;
	}

	/*子进程使用execl将当前进程地址空间的内容替换为raspistill，后面跟的是参数*/
	if(forkRet==0){
		int execlRet=execl("/usr/bin/raspistill","raspistill","-o","garbage.jpg","-t","100",nullptr);
		if(execlRet<0){
			perror("execl error");
			exit(-1);
		}
	}else{
		/*父进程，阻塞在这里，等待子进程退出并回收子进程*/
		wait(nullptr);
		std::cout<<"Child process exit,he should take a picture named 'garbage.jpg'\n"<<std::endl;
	}
	return 0;
}

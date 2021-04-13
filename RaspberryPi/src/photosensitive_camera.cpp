#include "photosensitive_camera.h"

/*我的光敏电阻模块坏掉了，临时用键盘上的a键触发*/
#define HIT_A_TO_TRIGER

int waitForMyPic(void){
	if(wiringPiSetup()==-1){
		printf("setup wirginPi failed!\n");
		return -1;
	}

	pinMode(DO,INPUT);

#ifndef HIT_A_TO_TRIGER
	while(1){
		/*block here,wait for DO==1*/
		while(digitalRead(DO)!=1);
		sleep(1);
		/*after 1s,if DO is still 1,break to fork a child process and take a picture*/
		if(digitalRead(DO)==1)
			break;
	}
#else
	while(1){
		char triger='\0';
		scanf("%c",&triger);

		if(triger=='a')
			break;
		else
			continue;
	}
#endif

		int ret=fork();
		if(ret<0){
			perror("fork error");
			return -1;
		}

		/*child process*/
		if(ret==0){
			if(execl("/usr/bin/raspistill","raspistill","-o","garbage.jpg","-t","100",NULL)<0){
				perror("execl error");
				exit(0);
			}
		}else{
			/*parent process:block here,wait for child process exit*/
			wait(NULL);
			printf("Child process exit,he should take a picture named 'garbage.jpg'\n");
		}

		return 0;
	}


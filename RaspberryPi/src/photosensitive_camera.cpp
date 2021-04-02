#include <../include/photosensitive_camera.h>

int waitForMyPic(void){
	if(wiringPiSetup()==-1){
		printf("setup wirginPi failed!\n");
		return -1;
	}

	pinMode(DO,INPUT);

	while(1){
		while(digitalRead(DO)!=1);
		sleep(1);
		if(digitalRead(DO)==1)
			break;
	}

	int ret=fork();
	if(ret<0){
		perror("fork error");
		return -1;
	}

	if(ret==0){
		//char* execv_arg[]={"raspistill","-o","garbage.jpg","-t","100",NULL};
		if(execl("/usr/bin/raspistill","raspistill","-o","garbage.jpg","-t","100",NULL)<0){
			perror("execv error");
			exit(0);
		}
	}else{
		wait(NULL);
		printf("Child process exit,he has taken a picture named 'garbage.jpg'\n");
	}

	return 0;
}


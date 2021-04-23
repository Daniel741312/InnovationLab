#include "stepmotor.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){

	int orientation=atoi(argv[1]);
	int steps=atoi(argv[2]);
	printf("orientation=%d,steps=%d\n",orientation,steps);

	initial();

	if(orientation==0){
		forward(steps);
	}else{
		backward(steps);
	}

	stop();

	return 0;
}


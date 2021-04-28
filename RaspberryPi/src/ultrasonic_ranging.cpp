#include "ultrasonic_ranging.h"
#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

/*
#define TRIG 4
#define ECHO 5
*/

namespace ultrasonic_ranging{

UltraSonicRanging::UltraSonicRanging(int ECHO,int TRIG){
    if (wiringPiSetup() == -1) {
        write(STDERR_FILENO, "Setup wiringPi failed\nexit now\n", 32);
        exit(-1);
    }
    echo_=ECHO;
    trig_=TRIG;
    pinMode(echo_, INPUT);
    pinMode(trig_, OUTPUT);
    printf("A new UltraSonicRanging:ECHO-%d,TRIG-%d\n",echo_,trig_);
}

UltraSonicRanging::~UltraSonicRanging(){
    printf("UltraSonicRanging has been removed\n");
}

float UltraSonicRanging::DisMeasure(){
    /*linux系统自带的时间结构体*/
    struct timeval tv1;
    struct timeval tv2;
    long start, stop;
    float dis=0;

    /*TRIG写2us的低电平*/
    digitalWrite(trig_, LOW);
    delayMicroseconds(2);

    /*TRIG写10us的高电平*/
    digitalWrite(trig_, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_, LOW);

    /*
            *TRIG信号：
            *	|----------|
                |          |
              --		    ----
    */

    /*ECHO==1的时刻是发射ultrasonic的时刻，只要Echo!=1，就死循环阻塞等待在这儿*/
    while (digitalRead(echo_) != 1);
    /*跳出上面的循环时，就是ultrasonic发射的时刻*/
    gettimeofday(&tv1, nullptr);

    /*同理，tv2是接收到回射超声波的时刻*/
    while (digitalRead(echo_) != 0);
    gettimeofday(&tv2, nullptr);

    start = tv1.tv_sec * 1e6+ tv1.tv_usec;
    stop = tv2.tv_sec * 1e6 + tv2.tv_usec;

    dis = static_cast<float>((stop - start) / 1e6* 3.4e4/ 2);

    return dis;
}

}

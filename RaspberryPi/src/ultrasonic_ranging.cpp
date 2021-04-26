#include "ultrasonic_ranging.h"

#define TRIG 4
#define ECHO 5

/*设置4号引脚输出触发信号，5号引脚输入返回的信号*/
int ultraInit(void) {
    if (-1 == wiringPiSetup()) {
        std::cerr << "Setup wiringPi failed" << std::endl;
        return -1;
    }
    pinMode(ECHO, INPUT);
    pinMode(TRIG, OUTPUT);
    return 0;
}

float disMeasure(void) {
    /*linux系统自带的时间结构体*/
    struct timeval tv1;
    struct timeval tv2;
    long start, stop;
    float dis;

    /*TRIG写2us的低电平*/
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    /*TRIG写10us的高电平*/
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    /*
            *TRIG信号：
            *	|----------|
                    |          |
              --		    ----
    */

    /*ECHO==1的时刻是发射ultrasonic的时刻，只要Echo!=1，就死循环阻塞等待在这儿*/
    while (digitalRead(ECHO) != 1)
        ;
    /*跳出上面的循环时，就是ultrasonic发射的时刻*/
    gettimeofday(&tv1, NULL);

    /*同理，tv2是接收到回射超声波的时刻*/
    while (digitalRead(ECHO) != 0)
        ;
    gettimeofday(&tv2, NULL);

    start = tv1.tv_sec * 1000000 + tv1.tv_usec;
    stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

    dis = (float)(stop - start) / 1000000 * 34000 / 2;

    return dis;
}

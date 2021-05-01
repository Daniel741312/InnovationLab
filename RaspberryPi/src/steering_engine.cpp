#include "steering_engine.h"

#include <unistd.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

/*
#define SERVO 26
*/
namespace steering_engine {

SteerEngine::SteerEngine(int PWM) {
    if (wiringPiSetup() == -1) {
        write(STDERR_FILENO, "Setup wiringPi failed\nexit now\n", 32);
        exit(-1);
    }
    this->pwm_pin_ = PWM;
    pinMode(pwm_pin_, OUTPUT);
    printf("A new SteerEngine:PWM-%d\n", pwm_pin_);
}

SteerEngine::~SteerEngine() { printf("SteerEngine has been removed\n"); }

/*
    0.5ms--0度；
    1.0ms--45度；
    1.5ms--90度；
    2.0ms--135度；
    2.5ms--180度；
    占空每增加0.5ms，就多转过45度
    由于操作系统进程调度的关系，软件产生的pwm信号肯定不准，不过这里精度要求不高
*/

void SteerEngine::RotateByAngle(int angle) {
    int duty = static_cast<int>(500 * static_cast<double>(angle) / 45 + 500);
    /*
        跑一圈for循环需要20ms（至少），这里有瑕疵：
        不论转过多少角度都要跑400ms，
        实际上迭代的次数应该与转过的角度成正比
    */
    for (int i = 0; i < 20; i++) {
        digitalWrite(pwm_pin_, HIGH);
        delayMicroseconds(duty);
        digitalWrite(pwm_pin_, LOW);
        delayMicroseconds(2e4 - duty);
    }
    return;
}

}  // namespace steering_engine

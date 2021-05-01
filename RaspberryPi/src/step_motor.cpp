#include "step_motor.h"

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <stdlib.h>

namespace step_motor {

StepMotor::StepMotor(int IN1, int IN2, int IN3, int IN4, int interval) {
    if (wiringPiSetup() == -1) {
        write(STDERR_FILENO, "Setup wiringPi failed\nexit now\n", 32);
        exit(-1);
    }

    in1_ = IN1;
    in2_ = IN2;
    in3_ = IN3;
    in4_ = IN4;
    interval_= interval;

    /*设置四个引脚为输出模式*/
    pinMode(in1_, OUTPUT);
    pinMode(in2_, OUTPUT);
    pinMode(in3_, OUTPUT);
    pinMode(in4_, OUTPUT);

    printf("A new StepMotor:IN1-%d,IN2-%d,IN3-%d,IN4-%d,delay-%d\n", in1_, in2_, in3_, in4_, interval_);
}

StepMotor::~StepMotor() { 
    SetStep(0, 0, 0, 0);
    printf("StepMotor has been removed\n"); 
}

void StepMotor::RotateClockwise(int steps) {
    for (int i = 0; i < steps; i++) {
        SetStep(1, 0, 0, 0);
        delay(interval_);
        SetStep(0, 1, 0, 0);
        delay(interval_);
        SetStep(0, 0, 1, 0);
        delay(interval_);
        SetStep(0, 0, 0, 1);
        delay(interval_);
    }
    return;
}

void StepMotor::RotateAntiClockwise(int steps) {
    for (int i = 0; i < steps; i++) {
        SetStep(0, 0, 0, 1);
        delay(interval_);
        SetStep(0, 0, 1, 0);
        delay(interval_);
        SetStep(0, 1, 0, 0);
        delay(interval_);
        SetStep(1, 0, 0, 0);
        delay(interval_);
    }
    return;
}

void StepMotor::Stop(void) {
    SetStep(0, 0, 0, 0);
    return;
}

void StepMotor::setIN1_IN4(int IN1, int IN2, int IN3, int IN4) {
    this->in1_ = IN1;
    this->in2_ = IN2;
    this->in3_ = IN3;
    this->in4_ = IN4;
    return;
}

inline void StepMotor::SetStep(int a, int b, int c, int d) {
    digitalWrite(in1_, a);
    digitalWrite(in2_, b);
    digitalWrite(in3_, c);
    digitalWrite(in4_, d);
}

}  // namespace step_motor

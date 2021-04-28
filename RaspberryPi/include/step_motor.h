#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

/*
#define IN1 22
#define IN2 23
#define IN3 24
#define IN4 25
#define DELAY 4
*/

namespace step_motor{

class StepMotor {
   public:
    StepMotor(int IN1, int IN2, int IN3, int IN4, int interval= 4);
    ~StepMotor();

    void RotateClockwise(int steps);
    void RotateAntiClockwise(int steps);
    void Stop(void);

    void setIN1_IN4(int IN1, int IN2, int IN3, int IN4);

    /*步进电机四个引脚连接到树莓派上的GPIO的编号*/
    int in1_;
    int in2_;
    int in3_;
    int in4_;
    /*控制转速*/
    int interval_= 4;

   private:
    /*向四个引脚写入电平值，大量调用，定义为了内联函数*/
    void SetStep(int a, int b, int c, int d);
};

}  // namespace stpmtr

#endif

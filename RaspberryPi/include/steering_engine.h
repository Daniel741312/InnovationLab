#ifndef __STEERING_ENGINE_H__
#define __STEERING_ENGINE_H__

namespace steering_engine{

    class SteerEngine{
        public:
            explicit SteerEngine(int PWM=26);
            ~SteerEngine();

            void RotateByAngle(int angle);
            int pwm_pin_=26;
    };

}

#endif
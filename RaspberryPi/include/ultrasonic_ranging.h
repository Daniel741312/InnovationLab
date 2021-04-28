#ifndef __ULTRASONIC_RANGING_H__
#define __ULTRASONIC_RANGING_H__

namespace ultrasonic_ranging {

class UltraSonicRanging {
   public:
    UltraSonicRanging(int ECHO, int TRIG);
    ~UltraSonicRanging();
    float DisMeasure();
    int echo_;
    int trig_;
};

}  // namespace ultrasonic_ranging

#endif

#include "servo_motor.h"

int throwGarbage(void) {
    if (wiringPiSetup()) {
        std::cerr << "Setup wiringPi failed" << std::endl;
        return -1;
    }
    pinMode(SERVO, OUTPUT);

    int cnt = 20;

    while (cnt != 0) {
        digitalWrite(SERVO, HIGH);
        delayMicroseconds(1500);
        digitalWrite(SERVO, LOW);
        delayMicroseconds(18500);
        cnt--;
    }

    delay(500);

    while (cnt != 10) {
        digitalWrite(SERVO, HIGH);
        delayMicroseconds(500);
        digitalWrite(SERVO, LOW);
        delayMicroseconds(19500);
        cnt++;
    }

    return 0;
}

#include "pwm.h"
#include "unistd.h"

#define servo_min 900
#define servo_max 1450
#define gain_0 250
#define gain_1 75
#define gain_2 250
#define gain_3 75

int main(int argc, char **argv)
{
    initPWM(0x40);

    setPWMFreq(2000);
    resetAllPWM(0, 900);
    usleep(100000);

    setPWM(0, 0, (servo_min + gain_0));
    setPWM(1, 0, (servo_min + gain_1));
    setPWM(2, 0, (servo_min + gain_2));
    setPWM(3, 0, (servo_min + gain_3));

    int x = servo_min + 40;
    while (x <= servo_max)
    {
        setPWM(0, 0, (x + gain_0));
        setPWM(1, 0, (x + gain_1));
        setPWM(2, 0, (x + gain_2));
        setPWM(3, 0, (x + gain_3));
        x++;
        usleep(2000);
    }

    usleep((1000*1000) * 3);

    while (x >= servo_min)
    {
        setPWM(0, 0, (x + gain_0));
        setPWM(1, 0, (x + gain_1));
        setPWM(2, 0, (x + gain_2));
        setPWM(3, 0, (x + gain_3));
        x -= 5;
        usleep(500);
    }
}

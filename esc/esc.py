import time
import Adafruit_PCA9685
 
# Initialise the PCA9685 using the default address (0x40).
pwm = Adafruit_PCA9685.PCA9685()

frequency = 2000
servo_min = 900
servo_max = 1400
gain_0 = 0
gain_1 = 75
gain_2 = 0
gain_3 = 75

# Helper function to make setting a servo pulse width simpler.
def command_all_motors(val):
    pwm.set_pwm(0, 0, val + gain_0)
    pwm.set_pwm(1, 0, val + gain_1)
    pwm.set_pwm(2, 0, val + gain_2)
    pwm.set_pwm(3, 0, val + gain_3)

def stop_all():
    pwm.set_pwm(0, 0, servo_min)
    pwm.set_pwm(1, 0, servo_min)
    pwm.set_pwm(2, 0, servo_min)
    pwm.set_pwm(3, 0, servo_min)

def setup():
    pwm.set_pwm_freq(frequency)
    time.sleep(1)
    print("Set frequency to ", frequency)
    pwm.set_pwm(0, 0, servo_min + gain_0)
    pwm.set_pwm(1, 0, servo_min + gain_1)
    pwm.set_pwm(2, 0, servo_min + gain_2)
    pwm.set_pwm(3, 0, servo_min + gain_3)
    time.sleep(2)
    print("Setup complete")

setup()
x = servo_min + 40
while x <= servo_max: # Spin up
    command_all_motors(x)
    print("Level:", str(x))
    x += 1
    time.sleep(.02)

time.sleep(1)

while x >= servo_min: # Spin down
    command_all_motors(x)
    print("Level:", str(x))
    x -= 5
    time.sleep(.005)

stop_all()
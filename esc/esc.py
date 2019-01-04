# Simple demo of of the PCA9685 PWM servo/LED controller library.
# This will move channel 0 from min to max position repeatedly.
# Author: Tony DiCola
# License: Public Domain
from __future__ import division
import time

# Import the PCA9685 module.
import Adafruit_PCA9685
 
# Uncomment to enable debug output.
#import logging
#logging.basicConfig(level=logging.DEBUG)

# Initialise the PCA9685 using the default address (0x40).
pwm = Adafruit_PCA9685.PCA9685()

# Alternatively specify a different address and/or bus:    
#pwm = Adafruit_PCA9685.PCA9685(address=0x41, busnum=2)

# Configure min and max servo pulse lengths
servo_min = 800  # Min pulse length out of 4096
servo_max = 1300  # Max pulse length out of 4096

# Helper function to make setting a servo pulse width simpler.
def command_all_motors(val):
    # pwm.set_pwm(0, 0, val)
    # pwm.set_pwm(1, 0, val)
    pwm.set_pwm(2, 0, val)
    pwm.set_pwm(3, 0, val)

def setup():
    # pwm.set_pwm(0, 0, servo_min)
    # pwm.set_pwm(1, 0, servo_min)
    pwm.set_pwm(2, 0, servo_min)
    pwm.set_pwm(3, 0, servo_min)

# Set frequency to 60hz, good for servos.
frequency = 2000
pwm.set_pwm_freq(frequency)
time.sleep(1)
print("Set frequency to ", frequency)

#I am using channel 8 because I broke channel 0 while soldering. rip. luckily nothing else is broke though.
setup()
time.sleep(2)
# pwm.set_pwm(0, 0, servo_max)
# time.sleep(2)
print("Configured")
#gives time for ESC to recogize signal.

x = servo_min + 40
while x <= servo_max:

    command_all_motors(x)
    print("Level:", str(x))
    x += 1
    time.sleep(.05)

while x >= servo_min:
    command_all_motors(x)
    print("Level:", str(x))
    x -= 1
    time.sleep(.01)

command_all_motors(servo_min)
import time
import Adafruit_ADS1x15

# Channel definitions
YAW = 0
THROTTLE = 1
ROLL = 2
PITCH = 3

GAIN = 1

def init():
    return Adafruit_ADS1x15.ADS1115()

def get_position(read_val):
    DIVISOR = 100
    OFFSET = -130
    NORMALIZER = (10 / 7)
    tmp = (read_val / 100) + OFFSET
    position = float(tmp * NORMALIZER)
    return position

def run(controller):
    # Print nice channel column headers.
    print('|    YAW |  THRTL |   ROLL |  PITCH |')
    print('-' * 37)
    values = [0]*4
    # Main loop.
    while True:
        # Read all the controller channel values in a list.
        values[YAW] = get_position(controller.read_adc(YAW, gain=GAIN))
        values[THROTTLE] = get_position(controller.read_adc(THROTTLE, gain=GAIN))
        values[ROLL] = get_position(controller.read_adc(ROLL, gain=GAIN))
        values[PITCH] = get_position(controller.read_adc(PITCH, gain=GAIN))
        print('| {0:>6} | {1:>6} | {2:>6} | {3:>6} |'.format(*values))
        time.sleep(0.25)

if __name__ == "__main__":
    controller = init()
    run(controller)

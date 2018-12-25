import time
import Adafruit_ADS1x15
import os
import struct

# Channel definitions
YAW = 0
THROTTLE = 1
PITCH = 2
ROLL = 3

# Offsets
YAW_OFFSET = -131
THROTTLE_OFFSET = -130
ROLL_OFFSET = -130
PITCH_OFFSET = -130
DIVISOR = 100

GAIN = 1

fifo_name = "/tmp/ctrl_fifo"

def init():
    try:
        os.mkfifo(fifo_name)
    except FileExistsError:
        pass

    return Adafruit_ADS1x15.ADS1115()

def _normalize_position(adjusted_read_value):
    NORMALIZER = (10.0 / 7.0)
    position = float(adjusted_read_value * NORMALIZER)
    return position

def calc_position(read_val, offset):
    adjusted_read_value = (read_val / DIVISOR) + offset
    return _normalize_position(adjusted_read_value)

def read_controller(channel):
    return controller.read_adc(channel, gain=GAIN, data_rate=250)

def run(controller):
    # Print nice channel column headers.
    # print('|    YAW |  THRTL |   ROLL |  PITCH |')
    # print('-' * 37)
    values = [0]*4

    # Main loop.
    while True:
        # Read all the controller channel values in a list.
        values[YAW] = calc_position(read_controller(YAW), YAW_OFFSET)
        values[THROTTLE] = calc_position(read_controller(THROTTLE), THROTTLE_OFFSET)
        values[ROLL] = calc_position(read_controller(ROLL), ROLL_OFFSET)
        values[PITCH] = calc_position(read_controller(PITCH), PITCH_OFFSET)
        # output = "{} | {} | {} | {}".format(values[YAW], values[THROTTLE], values[ROLL], values[PITCH])
        # print(output)
        with open(fifo_name, 'wb') as f:
            buf = struct.pack('%sf' % len(values), *values)
            f.write(buf)
        # time.sleep(1)

if __name__ == "__main__":
    controller = init()
    run(controller)

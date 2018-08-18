import RPi.GPIO as io
import time

# Suggested setup:
# Green led     pin 18
# Yellow led    pin 23
# Red led       pin 24

# 330 Ohm resisters from ground to short side of leds
# jumpers from pin n to long side of leds

GREEN = 18
YLW = 23
RED = 24
DELAY = 0.15

io.setmode(io.BCM)
io.setwarnings(False)
io.setup(GREEN, io.OUT)
io.setup(YLW, io.OUT)
io.setup(RED, io.OUT)

print("Starting leds...")
while True:
    io.output(GREEN, io.HIGH)
    io.output(YLW, io.LOW)
    io.output(RED, io.LOW)
    time.sleep(2)
    io.output(GREEN, io.LOW)
    io.output(YLW, io.HIGH)
    io.output(RED, io.LOW)
    time.sleep(1)
    io.output(GREEN, io.LOW)
    io.output(YLW, io.LOW)
    io.output(RED, io.HIGH)
    time.sleep(10)
    # io.output(GREEN, io.LOW)
    # io.output(YLW, io.HIGH)
    # io.output(RED, io.LOW)
    # time.sleep(DELAY)


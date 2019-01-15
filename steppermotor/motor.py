import RPi.GPIO as GPIO
import time
import sys,tty,termios


COIL_0 = 6
COIL_1 = 13
COIL_2 = 19
COIL_3 = 26
PULSE_DELAY = 0.006


class _Getch:
	def __call__(self):
		fd = sys.stdin.fileno()
		old_settings = termios.tcgetattr(fd)
		try:
			tty.setraw(sys.stdin.fileno())
			ch = sys.stdin.read(3)
		finally:
			termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
		return ch


def setup():
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(COIL_0, GPIO.OUT)
	GPIO.setup(COIL_1, GPIO.OUT)
	GPIO.setup(COIL_2, GPIO.OUT)
	GPIO.setup(COIL_3, GPIO.OUT)


def forward(degrees):
	steps = degrees_to_steps(degrees)
	for i in range(0, steps):
		set_step(1, 0, 0, 0)
		set_step(0, 1, 0, 0)
		set_step(0, 0, 1, 0)
		set_step(0, 0, 0, 1)


def backward(degrees):
	steps = degrees_to_steps(degrees)
	for i in range(0, steps):
		set_step(0, 0, 0, 1)
		set_step(0, 0, 1, 0)
		set_step(0, 1, 0, 0)
		set_step(1, 0, 0, 0)


def set_step(enable_0, enable_1, enable_2, enable_3):
	GPIO.output(COIL_0, enable_0)
	GPIO.output(COIL_1, enable_1)
	GPIO.output(COIL_2, enable_2)
	GPIO.output(COIL_3, enable_3)
	time.sleep(PULSE_DELAY)


def degrees_to_steps(degrees):
	ratio = 512.0 / 360.0 # 512 steps per revolution
	steps = float(degrees) * ratio
	return int(steps)


# Keyboard left/down enable CCW rotation
# Keyboard right/up enable CW rotation
# Any other key exits
def main():
	while(True):
		inkey = _Getch()
		while(1):
			k=inkey()
			if k!='':break
		if k == '\x1b[A':
			# print "up"
			forward(1)
		elif k == '\x1b[B':
			# print "down"
			backward(1)
		elif k == '\x1b[C':
			# print "right"
			forward(1)
		elif k == '\x1b[D':
			# print "left"
			backward(1)
		else:
			return


if __name__ == "__main__":
	try:
		setup()
		main()
	finally:
		GPIO.cleanup()

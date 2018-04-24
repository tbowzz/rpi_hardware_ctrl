import RPi.GPIO as GPIO
import time

def getDistance():

	# Configuration
	print("Setting up...")
	GPIO.setmode(GPIO.BCM)
	TRIG = 23
	ECHO = 24
	GPIO.setup(TRIG, GPIO.OUT)
	GPIO.setup(ECHO, GPIO.IN)
	GPIO.output(TRIG,False)
	time.sleep(2)
	print("Done.")

	while True:
		GPIO.output(TRIG, True)
		time.sleep(0.00001)
		GPIO.output(TRIG, False)

		while GPIO.input(ECHO) == 0:
			pass
		pulse_start = time.time()

		while GPIO.input(ECHO) == 1:
			pass
		pulse_end = time.time()

		pulse_duration = pulse_end - pulse_start
		distance = pulse_duration * 17150
		print ("Distance: %.1f cm" % distance)
		time.sleep(0.4)

if __name__ == '__main__':
	try:
		getDistance()
 
	except KeyboardInterrupt:
		GPIO.cleanup()

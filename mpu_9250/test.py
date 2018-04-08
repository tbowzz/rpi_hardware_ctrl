#!/usr/bin/python

import smbus
import math
import time

# Sensor registers
ACCEL_X = 0x3b # 59
ACCEL_Y = 0x3d # 61
ACCEL_Z = 0x3f # 63

TEMP = 0x41 #65

GYRO_X = 0x43 # 67
GYRO_Y = 0x45 # 69
GYRO_Z = 0x47 # 71

# Scalars
def ACCEL_SCALE(val):
	return val / 16384.0
def GYRO_SCALE(val):
	return val / 131

# Power management registers
power_mgmt_1 = 0x6b
power_mgmt_2 = 0x6c

def read_byte(adr):
    return bus.read_byte_data(address, adr)

def read_word(adr):
    high = bus.read_byte_data(address, adr)
    low = bus.read_byte_data(address, adr+1)
    val = (high << 8) + low
    return val

def read_word_2c(adr):
    val = read_word(adr)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val

def dist(a,b):
    return math.sqrt((a*a)+(b*b))

def get_y_rotation(x,y,z):
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def get_x_rotation(x,y,z):
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

bus = smbus.SMBus(1) # or bus = smbus.SMBus(1) for Revision 2 boards
address = 0x68       # This is the address value read via the i2cdetect command

# Wake it up since it starts in sleep mode
bus.write_byte_data(address, power_mgmt_1, 0)

while True:
	print "------------------"
	print "\nGYRO"
	gyro_xout = read_word_2c(GYRO_X)
	gyro_yout = read_word_2c(GYRO_Y)
	gyro_zout = read_word_2c(GYRO_Z)

	print "gyro_xout: ", gyro_xout, " scaled: ", (GYRO_SCALE(gyro_xout))
	print "gyro_yout: ", gyro_yout, " scaled: ", (GYRO_SCALE(gyro_yout))
	print "gyro_zout: ", gyro_zout, " scaled: ", (GYRO_SCALE(gyro_zout))

	print "ACCELEROMETER"
	accel_xout = read_word_2c(ACCEL_X)
	accel_yout = read_word_2c(ACCEL_Y)
	accel_zout = read_word_2c(ACCEL_Z)

	accel_xout_scaled = ACCEL_SCALE(accel_xout)
	accel_yout_scaled = ACCEL_SCALE(accel_yout)
	accel_zout_scaled = ACCEL_SCALE(accel_zout)

	print "accel_xout: ", str(accel_xout).rjust(8), " scaled: {0:.6f}".format(accel_xout_scaled)
	print "accel_yout: ", str(accel_yout).rjust(8), " scaled: {0:.6f}".format(accel_yout_scaled)
	print "accel_zout: ", str(accel_zout).rjust(8), " scaled: {0:.6f}".format(accel_zout_scaled)
	
	print "ROTATION"
	print "x: " , get_x_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled)
	print "y: " , get_y_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled)

	print "TEMPERATURE"
	temp_out = read_word_2c(TEMP)

	# This is wrong...
	print "temp: ", (temp_out / 6.85)
	
	print "------------------"
	time.sleep(1)
#!/usr/bin/python

import serial
import pynmea2
import sys

# http://navspark.mybigcommerce.com/content/NMEA_Format_v0.1.pdf
# https://github.com/Knio/pynmea2/
# This was super important
# http://wiki.dragino.com/index.php?title=Getting_GPS_to_work_on_Raspberry_Pi_3_Model_B

M_TO_FT = 0.328084
LATLON_CONVERT = 100.0

GPS = serial.Serial('/dev/ttyS0', baudrate=9600)

class GpsData(object):
        def __init__(self):
                self.num_sats = 0
                self.alt = -1.0
                self.lat_dir = ""
                self.lon_dir = ""
                self.lat = -1.0
                self.lon = -1.0
                self.timestamp = 0
                self.speed = 0
        def print_data(self):
                print("===============")
                print("sats: " + str(self.num_sats))
                print("alt: " + str(self.alt) + " ft")
                print("lat: " + str(self.lat) + " " + self.lat_dir)
                print("lon: " + str(self.lon) + " " + self.lon_dir)
                print("time: " + str(self.timestamp))
                # print("speed: " + str(self.speed))

def main():
        while(1):
                while GPS.inWaiting()==0:
                        pass
                NMEA=GPS.readline()
                parse_gps(NMEA)

def parse_gps(nmea):
        if "GNRMC" in nmea:
                data = str(nmea).split(',')
                if data[2] is 'A': # if data is valid
                        speed = data[7]
                        if float(speed) < 1.5:
                                speed = 0.0
                        print("speed: " + str(speed) + " mph")
                return
        elif "GNGGA" not in nmea:
                # we don't need these other strings
                return
        try:
                msg = pynmea2.parse(nmea)
                data = GpsData()
                data.num_sats = msg.num_sats
                data.alt = msg.altitude / M_TO_FT

                data.lat_dir = str(msg.lat_dir)
                data.lon_dir = str(msg.lon_dir)

                lat_dec_degrees = int(float(msg.lat)/100)
                lat_sec = float(msg.lat) - lat_dec_degrees * 100
                data.lat = lat_dec_degrees + lat_sec / 60
                if data.lat_dir is "S":
                        data.lat *= -1

                lon_dec_degrees = int(float(msg.lon)/100)
                lon_sec = float(msg.lon) - lon_dec_degrees * 100
                data.lon = lon_dec_degrees + lon_sec / 60
                if data.lon_dir is "W":
                        data.lon *= -1

                data.timestamp = msg.timestamp

                data.print_data()
        except:
                pass


if __name__ == "__main__":
    main()

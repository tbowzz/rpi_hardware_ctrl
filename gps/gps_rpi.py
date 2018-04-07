#!/usr/bin/python3

import serial
# Abandoning in python for now because RPi.GPIO doesn't yet support UART
# from RPi.GPIO import *
# import pynmea2
import sys

# http://navspark.mybigcommerce.com/content/NMEA_Format_v0.1.pdf
# https://github.com/Knio/pynmea2/

M_TO_FT = 0.328084
LATLON_CONVERT = 100.0

# UART.setup("UART1")
GPS = serial.Serial('/dev/ttyAMA0', baudrate=9600)

class GpsData(object):
        def __init__(self):
                self.num_sats = 0
                self.alt = -1.0
                self.lat_dir = ""
                self.lon_dir = ""
                self.lat = -1.0
                self.lon = -1.0
                self.timestamp = 0
        def print_data(self):
                print("===============")
                print("sats: " + str(self.num_sats))
                print("alt: " + str(self.alt) + " ft")
                print("lat: " + str(self.lat) + " " + self.lat_dir)
                print("lon: " + str(self.lon) + " " + self.lon_dir)
                print("time: " + str(self.timestamp))

def main():
        while(1):
                while GPS.inWaiting()==0:
                        pass
                NMEA=GPS.readline()
                print(NMEA)
                # parse_gps(NMEA)

def parse_gps(nmea):
        if "GNRMC" in nmea:
                # TODO: get speed from this
                # msg = pynmea2.parse(nmea)
                # print(str(msg))
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
                if data.lat_dir is "N":
                        data.lat = float(msg.lat) / LATLON_CONVERT
                else:
                        data.lat = (float(msg.lat) / LATLON_CONVERT) * -1
                if data.lon_dir is "E":
                        data.lon = float(msg.lon) / LATLON_CONVERT
                else:
                        data.lon = (float(msg.lon) / LATLON_CONVERT) * -1
                data.timestamp = msg.timestamp

                data.print_data()
        except:
                pass


if __name__ == "__main__":
    main()

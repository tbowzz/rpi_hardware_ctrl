#!/bin/bash

ifconfig wlan1 down
iwconfig wlan1 mode monitor
# iwconfig wlan1 channel 1
# iwconfig wlan1 rate 54M
ifconfig wlan1 up

#!/usr/bin/python3

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(13,GPIO.OUT)

print("Unlocking door.")
GPIO.output(13,GPIO.HIGH)

print("Waiting 5sec.") 
time.sleep(5)

print("Locking door again.")
GPIO.output(13,GPIO.LOW)

GPIO.cleanup()

import serial
import sys

try:
    ser = serial.Serial('/dev/ttyUSB0')
except Exception:
    print ("open serial failed")
    exit(1)
print ("A Serial Echo is Running...")

while True:
    # echo
    s = ser.read()  
    ser.write(s)
    # write to stdout and flush it
    sys.stdout.write(s)
    sys.stdout.flush()


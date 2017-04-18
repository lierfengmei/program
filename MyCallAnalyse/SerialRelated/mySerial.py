import serial
import sys


from time import sleep

ser = serial.Serial('/dev/ttyUSB0',9600,timeout=0.5)

#def recv(serial):
#    data
#    while True:
#        data = serial.read(30)
#        if data=='':
#            continue
#        else:
#            break
#        sleep(0.02)
#    return data
#    
#
#if __name__=="__main__":
#    while True:
#        data = recv(ser)
#        ser.write(data)



try:
    ser = serial.Serial('/dev/ttyUSB0',9600)
except Exception,e:
    print 'open serial failed.'
    exit(1)
print 'A Serial Echo Is Running...'
while True:
    # echo
    s = ser.read()
    ser.write(s)
    # write to stdout and flush it 
    sys.stdout.write(s)
    sys.stdout.flush()


# all parameters
ser = serial.Serial(
        port = None,
        baudrate = 9600,
        bytesize = EITHTBITS,
        parity = PARITY_NONE,
        stopbits = STOPBITS_ONE,
        timeout = None,
        xonxoff = 0,            # enable software flow control
        rtscts = 0,             # enable RTS/CTS flow control
        interCharTimeout = None
        )







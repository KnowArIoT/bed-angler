import serial
import sys

device = sys.argv[1]
motor = ord(sys.argv[2])
angle = int(sys.argv[3])

serialPort = serial.Serial(device, 9600, timeout=0.5)

serialPort.write(chr(motor))
serialPort.write(chr(angle))

serialPort.close()

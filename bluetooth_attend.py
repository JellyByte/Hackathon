import serial
import time
import random
student = range(0,10000)

serialPort = serial.Serial(port='COM4', baudrate=9600, timeout=10)

size1 = 7 # botton 1
size2 = 8 # botton 2
num = 0
last_press_time = time.time()
threshold = 2

try:
    while 1:
        data1 = serialPort.readline(size1).decode('utf-8').strip()
        data2 = serialPort.readline(size2).decode('utf-8').strip()
        if data1:
            print(data1)
            if data1 == "pressed":
                num += 1
                last_press_time = time.time()
        elif time.time() - last_press_time > threshold:
            print("Button not pressed for", threshold, "seconds.")
            break
except KeyboardInterrupt:
    pass
finally:
    serialPort.close()
    num += random.randint(0,10)
    print(student[num])
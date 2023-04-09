import serial
import time
import random

def roll_call(list):
    studentlist = list
    listnum = len(studentlist)

    serialPort = serial.Serial(port='COM4', baudrate=9600, timeout=10)

    size1 = 7 # botton 1 for Roll Call
    num = 0
    last_press_time = time.time()
    threshold = 2

    try:
        while 1:
            data1 = serialPort.readline(size1).decode('utf-8').strip()
            if data1:
                print(data1)
                if data1 == "pressed":
                    num += 1
                    if num == listnum-1: # index loop in the list
                        num = 0
                    last_press_time = time.time()
            elif time.time() - last_press_time > threshold:
                print("Button not pressed for", threshold, "seconds.")
                break
    except KeyboardInterrupt:
        pass
    finally:
        serialPort.close()
        num += random.randint(0,10)
        return studentlist[num]


def main():
    studentlist1 = range(0,2000)
    roll_call(studentlist1)

if __name__ == "__main__":
    main()
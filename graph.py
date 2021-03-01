import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import random
import serial

ser = serial.Serial('/dev/ttyUSB0', 115280)


if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters


fig, (ax1, ax2) = plt.subplots(2)
ax1.set_title('Temperature')
ax2.set_title('Pressure')
ax1.set(xlabel='n', ylabel='temp/C')
ax2.set(xlabel='n', ylabel='pressure/Pa')
ntemp = [] #number of temp measurements
npress = [] #number of pressure measurements
temp = [] #temperature values
press = [] #pressure values


def animate1(i, ntemp, temp):
    data = ser.readline()      
    data = data.decode("utf-8")
    data = data.replace('\r\n','')
    data = data.split(',')

    i = int(data[0])
    measure = float(data[1])

    ntemp.append(i)

    if data[2] == 't':
        temp.append(measure)
        ax1.plot(ntemp, temp, 'tab:green', label="Temperature")

def animate2(i, npress, press):
    data = ser.readline()      
    data = data.decode("utf-8")
    data = data.replace('\r\n','')
    data = data.split(',')

    i = int(data[0])
    measure = float(data[1])

    npress.append(i)
        
    if data[2] == 'p':
        press.append(measure)
        ax2.plot(npress, press, 'tab:orange', label="Pressure")
    


ani1 = animation.FuncAnimation(fig, animate1, fargs=(ntemp, temp), interval=500)
ani2 = animation.FuncAnimation(fig, animate2, fargs=(npress, press), interval=500)

plt.show()

import time
import random
import serial
import csv
import datetime

DateTime = 0


rows=0


bat = 0
Pressure = 0
Altitude = 0
Acc_X = 0
Acc_Y = 0
Acc_Z = 0
Long = 0
Lat = 0
head = 0
speed = 0

ser = serial.Serial("/dev/ttyUSB0",115200, timeout=1)

#csv dosyasının başlığı
'''with open('Car_data.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    field = ["Acc_X", "Acc_Y", "Acc_Z", "Gyr_X", "Gyr_Y", "Gyr_Z", "Pressure", "Altitude", "Nem", "sicaklik",
             "Longitude", "Latitude", "Hiz","Zaman_saat",]
    writer.writerow(field)'''

while(True):    
    data = ser.readline()
    data = data.decode("utf-8")
    if data:
        rows = [x for x in data.split(',')]
        bat = rows[0]
        Pressure = rows[1]
        Altitude = rows[2]
        Acc_X = rows[3]
        Acc_Y = rows[4]
        Acc_Z = rows[5]
        Long = rows[6]
        Lat = rows[7]
        head = rows[8]
        speed = rows[9]
        Zaman_saat = datetime.datetime.now()

        '''with open('Aviyonik_data.csv', 'a+', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([Acc_X, Acc_Y, Acc_Z, Gyr_X, Gyr_Y, Gyr_Z, Pressure, Altitude, Nem, sicaklik,Long, Lat, hiz,Zaman_saat,])'''
    print(rows)

from flask import Flask, Response, render_template
import cv2
import random
import time
import serial
import csv
import datetime
import sys

elapsed_distance = 0.0
previous_time = time.time()
voltage = 0
basinc = 0
yukseklik = 0
elapsed_time = 0
sure = 0
yaw = 0
pitch = 0
roll = 0
Longitude = 0
Latitude = 0
heading = 0
hiz =0
temp = 0



app = Flask(__name__)

def update_elapsed_distance(current_speed, previous_time):
    global elapsed_distance
    current_time = time.time()
    time_interval = current_time - previous_time
    distance_covered = current_speed * time_interval
    elapsed_distance += distance_covered
    return elapsed_distance, current_time

def update_elapsed_time():
    global elapsed_time
    global sure
    if 'start_time' not in globals():
        global start_time
        start_time = time.time()
    elapsed_time = time.time() - start_time
    sure = int(elapsed_time)

def generate_sensor_data():
    global voltage
    global basinc
    global yukseklik
    #ser = serial.Serial("COM22",115200, timeout=0.01)
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)

    while True:
        data = ser.readline()
        data = data.decode()
        if data:
            rows = [x for x in data.split(',')]
            voltage = rows[0]
            basinc = rows[1]
            yukseklik = rows[2]
            yaw = rows[3]
            pitch = rows[4]
            roll = rows[5]
            Longitude = rows[6]
            Latitude = rows[7]
            heading = rows[8]
            hiz =rows[9]
            temp = rows[10]
        # Simule la lecture des données de plusieurs capteurs
        #alinan_yol = round(random.uniform(20.0, 30.0), 2)  # Température en degrés Celsius
        #hiz = round(random.uniform(30.0, 70.0), 2)     # Humidité en pourcentage
        #heading = round(random.uniform(0.0, 100.0), 2)  # Pression en hPa
        #sure = round(random.uniform(0.0, 100.0), 2)  # Pression en pourcentage
        #basinc = round(random.uniform(950.0, 1050.0), 2)  # Température en degrés Celsius
        #yukseklik = round(random.uniform(30.0, 70.0), 2)     # Humidité en pourcentage
        #voltage = round(random.uniform(0.0, 100.0), 2)  # Pression en pourcentage
        #amper = round(random.uniform(0.0, 100.0), 2)  # Pression en pourcentage
        #power = round(random.uniform(0.0, 100.0), 2)  # Pression en pourcentage
        
        elapsed_distance, previous_time = update_elapsed_distance(hiz, previous_time)
        alinan_yol = int(elapsed_distance)
        
        # Format des données en JSON
        data = f"data: {{\"alinan_yol\": {alinan_yol}, \"hiz\": {hiz}, \"heading\": {heading}, \"sure\": {sure}, \"basinc\": {basinc}, \"yukseklik\": {yukseklik}, \"voltage\": {voltage}, \"amper\": {amper}, \"power\": {power}}}\n\n"
        
        # Yield les données formatées
        yield data
        
        # Attendre avant de générer une nouvelle donnée
        time.sleep(0.5)
        update_elapsed_time()

def generate_video():
    cap = cv2.VideoCapture(resource)
    rval, frame = cap.read()
    while rval:
        #cv2.imshow("Stream: " + resource_name, frame)
        rval, frame = cap.read()
        if not rval:
            break
        else:
            rval, buffer = cv2.imencode('.jpg', frame)
            frame = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

    cap.release()

@app.route('/sensor_data')
def sensor_data():
    return Response(generate_sensor_data(), content_type='text/event-stream')

@app.route('/video_feed')
def video_feed():
    return Response(generate_video(), content_type='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
def index():
    return render_template('index.html')

#if __name__ == '__main__':
#    app.run(debug=True, threaded=True)

if __name__ == '__main__':
    resource = sys.argv[1]
    app.run(host='0.0.0.0', port=80, debug=True, threaded = True)

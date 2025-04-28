from flask import Flask
import serial

app = Flask(__name__)

# SP config
SERIALP = '/dev/ttyUSB0'  #Use the device for Serial
BR = 9600              #Baud Rate

#Open Serial Port
ser = serial.Serial(SERIALP, BR, timeout=1)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=2222)


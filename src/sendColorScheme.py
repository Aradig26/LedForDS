import time
import serial
from networktables import NetworkTables

# Set up NetworkTables connection
NetworkTables.initialize(server='localhost')  # Replace 'localhost'
table = NetworkTables.getTable('Leds')

# Configure serial communication with Arduino Uno
arduino_port = '/dev/ttyACM0'  # Replace with Arduino port (windows: COMx)
baud_rate = 9600  # TODO: Fine tune baud rate
ser = serial.Serial(arduino_port, baud_rate, timeout=1)


# Function to send data to Arduino
def send_to_arduino(data):
    data_str = str(data)  # Ensure data is in string format
    ser.write(data_str.encode())  # Send data to Arduino


try:
    while True:
        # Read data from NetworkTables 'Leds/ColorScheme' field
        color = table.getString('ColorScheme', 'DEFAULT')

        # Send the color data to Arduino
        send_to_arduino(color)

except KeyboardInterrupt:
    ser.close()  # Close the serial connection on program termination

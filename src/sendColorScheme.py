import serial
from networktables import NetworkTables

# Set up NetworkTables connection
NetworkTables.initialize(server='roborio-467-frc.local')  # Replace with 'localhost' / 10.4.67.2 if it doesn't work
table = NetworkTables.getTable('Leds')

# Configure serial communication with Arduino Uno
arduino_port = 'COM5'  # Replace with Arduino port (windows: COMx)
baud_rate = 115200  # TODO: Fine tune baud rate
ser = serial.Serial(arduino_port, baud_rate, timeout=1)


# Function to send data to Arduino
def send_to_arduino(data):
    data_str = str(data)  # Ensure data is in string format
    ser.write(data_str.encode())  # Send data to Arduino


try:
    while True:
        # Read data from NetworkTables 'Leds/ColorScheme' field
        color = table.getString('ColorScheme', 'ERROR')  # Default to 'ERROR' if no data is found

        # Send the color data to Arduino
        send_to_arduino(color + '\n')
        print(ser.read().decode('utf-8'))  # Read data from Arduino

except KeyboardInterrupt:
    ser.close()  # Close the serial connection on program termination

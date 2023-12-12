import time
import serial
import logging
from networktables import NetworkTables

# Constants
SERVER = 'roborio-467-frc.local'
ARDUINO_PORT = 'COM5'
BAUD_RATE = 115200

# Set up logging
logging.basicConfig(level=logging.INFO)

# Set up NetworkTables connection
try:
    NetworkTables.initialize(server=SERVER)  # Replace with 'localhost' / 10.4.67.2 if it doesn't work
except Exception as e:
    logging.error(f'Failed to initialize NetworkTables with server {SERVER}: {e}.')

table = NetworkTables.getTable('Leds')


# Function to send data to Arduino
def send_to_arduino(serial, data):
    """Send data to Arduino via serial connection.

    Args:
    ser: The serial connection object.
    data: The data to send.
    """
    data_str = str(data)  # Ensure data is in string format
    try:
        serial.write(data_str.encode())  # Send data to Arduino
        logging.info('Data sent to Arduino successfully.')
    except Exception as error:
        logging.error(f'Failed to send data to Arduino: {error}.')


# Configure serial communication with Arduino Uno
try:
    ser = serial.Serial(ARDUINO_PORT, BAUD_RATE, timeout=1)
    logging.info('Successfully connected to Arduino.')
    while True:
        try:
            # Read data from NetworkTables 'Leds/ColorScheme' field
            color = table.getString('ColorScheme', 'ERROR')  # Default to 'ERROR' if no data is found
            send_to_arduino(ser, color)
            time.sleep(.25)  # Sleep for a bit to not thrash CPU

            # Read data from Arduino and log it
            logging.info('Data received from Arduino: {}'.format(ser.read().decode('utf-8')))
        except KeyboardInterrupt:
            ser.close()  # Close the serial connection on program termination
            logging.info('Serial connection to Arduino closed.')

except Exception as e:
    logging.error(f'Failed to establish connection with Arduino: {e}.')

import time
import serial
import logging
from networktables import NetworkTables

# Constants
SERVER = 'roborio-467-frc.local'
ARDUINO_PORT = 'COM6'
BAUD_RATE = 9600

# Set up logging
logging.basicConfig(level=logging.INFO)

# Set up NetworkTables connection
try:
    NetworkTables.initialize(server=SERVER)  # Replace with 'localhost' / 10.4.67.2 if it doesn't work
except Exception as e:
    logging.error(f'Failed to initialize NetworkTables with server {SERVER}: {e}.')

table = NetworkTables.getTable('AdvantageKit/RealOutputs/Leds')

# Mapping between color schemes and corresponding integer values
COLOR_SCHEME_MAP = {
    'WANT_CUBE': 100,
    'WANT_CONE': 101,
    'HOLD_CUBE': 102,
    'HOLD_CONE': 103,
    'INTAKE_CUBE': 104,
    'INTAKE_CONE': 105,
    'RELEASE_CUBE': 106,
    'RELEASE_CONE': 107,
    'BATTERY_LOW': 108,
    'ARM_UNCALIBRATED': 109,
    'CUBE_LOW': 110,
    'CUBE_MID': 111,
    'CUBE_HIGH': 112,
    'CONE_LOW': 113,
    'CONE_MID': 114,
    'CONE_HIGH': 115,
    'INTAKE_UNKNOWN': 116,
    'RELEASE_UNKNOWN': 117,
    'CALIBRATING': 118,
    'FLOOR': 119,
    'SHELF': 120,
    'BALANCE_VICTORY': 121,
    'AUTO_SCORE': 122,
    'IDLE': 123,
    'ERROR': 255  # Default value for unmatched cases
}


# Function to send data to Arduino
def send_to_arduino(ser, data):
    """Send data to Arduino via serial connection.

    Args:
    ser: The serial connection object.
    data: The data to send.
    """
    data_int = COLOR_SCHEME_MAP.get(data, COLOR_SCHEME_MAP['ERROR'])  # Get the corresponding integer or use default
    logging.info(data_int)
    try:
        ser.write([data_int])  # Send data to Arduino
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
            logging.info('Data received from Arduino: {}'.format(ser.read()))
        except KeyboardInterrupt:
            ser.close()  # Close the serial connection on program termination
            logging.info('Serial connection to Arduino closed.')

except Exception as e:
    logging.error(f'Failed to establish connection with Arduino: {e}.')

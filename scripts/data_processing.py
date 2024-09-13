import serial
from serial.tools.list_ports import comports

def list_ports(return_arduino_port=False):
    ports = comports()
    for port, desc, hwid in sorted(ports):
        print("{}: {} [{}]".format(port, desc, hwid))
    if return_arduino_port:
        arduino_port = None
        for port, desc, hwid in sorted(ports):
            if "Arduino" in desc:
                arduino_port = port
                break
        return arduino_port
    return ports

def check_baudrate(report_period_us, baudrate):
    bytes_per_report = 5
    bits_per_byte = 8
    required_baudrate = (1e6 / report_period_us) * bytes_per_report * bits_per_byte 
    print("Required baudrate: {:.0f}".format(required_baudrate))
    print("Selected baudrate: {:.0f}".format(baudrate))
    if baudrate < required_baudrate:
        print("WARNING: selected baudrate is too low.")
        return False
    else:
        print("Selected baudrate is sufficient.")
        return True

def readserial(port, baudrate, timeout, readcount, skip_head=1):
    data = []
    with serial.Serial(port, baudrate, timeout=timeout) as ser:
        for _ in range(skip_head):
            _ = ser.readline()
        for _ in range(readcount):
            line = ser.readline().decode("ASCII").strip()
            if '\r' in line:
                line = line.split('\r')
                for l in line:
                    data.append(l)
            else:
                data.append(line)
    return data
import serial

class Tunnel:
    def __init__(self, port):
        self.port = serial.Serial(port, 38400)
        print("Connecting (waiting for READY).")
        print("Please press reset when connecting takes too long!")
        print("Make sure to bridge pin 12 to GND")
        self.port.read_until(b'READY\n')

    def _recv(self):
        tmp = self.port.readline()
        # print("<-", tmp)
        return tmp

    def _send(self, b):
        # print("->", b)
        self.port.write(b)

    def command(self, command):
        self._send(str.encode(command + "\n"))
        return self._recv().decode()

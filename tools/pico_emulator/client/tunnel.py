import serial

class Tunnel:
    def __init__(self, port):
        self.port = serial.Serial(port, 115200, timeout=1)
        print("Connecting (waiting for READY).")
        # self.port.read_until(b'READY\n')
        self.command("PING")

    def _recv(self):
        tmp = self.port.readline()
        # print("<-", tmp)
        return tmp

    def _send(self, b):
        # print("->", b)
        self.port.write(b)

    def command(self, command):
        self._send(str.encode(command + "\n"))
        tmp = self._recv().decode()
        if tmp == "":
            raise Exception("Command failed")
        return tmp

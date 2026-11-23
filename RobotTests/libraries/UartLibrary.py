import os, sys
sys.path.insert(0, os.path.dirname(__file__))
from serial_common import SerialConnection


class UartLibrary:
    ROBOT_LIBRARY_SCOPE = "SUITE"

    def __init__(self, config_path=None):
        self.conn = SerialConnection(config_path)

    def connect(self):
        self.conn.connect()

    def disconnect(self):
        self.conn.disconnect()

    def send_text(self, text):
        resp = self.conn.send(f"uart send {text}")
        if "OK" not in resp:
            raise AssertionError(f"Expected OK, got: {resp}")
        return resp

    def receive_text(self):
        resp = self.conn.send("uart recv")
        if not resp.startswith("RX"):
            raise AssertionError(f"Expected RX, got: {resp}")
        return resp

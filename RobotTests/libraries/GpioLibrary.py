import os, sys
sys.path.insert(0, os.path.dirname(__file__))
from serial_common import SerialConnection


class GpioLibrary:
    ROBOT_LIBRARY_SCOPE = "SUITE"

    def __init__(self, config_path=None):
        self.conn = SerialConnection(config_path)

    def connect(self):
        self.conn.connect()

    def disconnect(self):
        self.conn.disconnect()

    def configure_pin(self, pin, mode):
        resp = self.conn.send(f"gpio config {pin} {mode}")
        if "OK" not in resp:
            raise AssertionError(f"Expected OK, got: {resp}")
        return resp

    def set_pin(self, pin, value):
        resp = self.conn.send(f"gpio set {pin} {value}")
        if "OK" not in resp:
            raise AssertionError(f"Expected OK, got: {resp}")
        return resp

    def get_pin(self, pin):
        resp = self.conn.send(f"gpio get {pin}")
        for line in resp.splitlines():
            if line.startswith("VAL"):
                return int(line.split()[1])
        raise AssertionError(f"Expected VAL, got: {resp}")

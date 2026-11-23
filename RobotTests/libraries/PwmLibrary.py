import os, sys
sys.path.insert(0, os.path.dirname(__file__))
from serial_common import SerialConnection


class PwmLibrary:
    ROBOT_LIBRARY_SCOPE = "SUITE"

    def __init__(self, config_path=None):
        self.conn = SerialConnection(config_path)

    def connect(self):
        self.conn.connect()

    def disconnect(self):
        self.conn.disconnect()

    def set_duty(self, channel, percent):
        resp = self.conn.send(f"pwm set {channel} {percent}")
        if "OK" not in resp:
            raise AssertionError(f"Expected OK, got: {resp}")
        return resp

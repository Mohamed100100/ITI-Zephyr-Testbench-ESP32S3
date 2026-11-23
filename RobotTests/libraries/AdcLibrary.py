import os, sys
sys.path.insert(0, os.path.dirname(__file__))
from serial_common import SerialConnection


class AdcLibrary:
    ROBOT_LIBRARY_SCOPE = "SUITE"

    def __init__(self, config_path=None):
        self.conn = SerialConnection(config_path)

    def connect(self):
        self.conn.connect()

    def disconnect(self):
        self.conn.disconnect()

    def read_channel(self, channel):
        resp = self.conn.send(f"adc read {channel}")
        for line in resp.splitlines():
            if line.startswith("VAL"):
                return int(line.split()[1])
        raise AssertionError(f"Expected VAL, got: {resp}")

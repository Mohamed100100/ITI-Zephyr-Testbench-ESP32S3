import os, sys
sys.path.insert(0, os.path.dirname(__file__))
from serial_common import SerialConnection
import re


class SpiLibrary:
    ROBOT_LIBRARY_SCOPE = "SUITE"

    def __init__(self, config_path=None):
        self.conn = SerialConnection(config_path)

    def connect(self):
        self.conn.connect()

    def disconnect(self):
        self.conn.disconnect()

    def transfer(self, *hex_bytes):
        cmd = "spi xfer " + " ".join(hex_bytes)
        resp = self.conn.send(cmd)
        if not resp.startswith("RX"):
            raise AssertionError(f"Expected RX, got: {resp}")
        return resp

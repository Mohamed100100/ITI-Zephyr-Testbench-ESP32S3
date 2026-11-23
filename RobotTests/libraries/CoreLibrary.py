import os, sys
sys.path.insert(0, os.path.dirname(__file__))
from serial_common import SerialConnection


class CoreLibrary:
    ROBOT_LIBRARY_SCOPE = "SUITE"

    def __init__(self, config_path=None):
        self.conn = SerialConnection(config_path)

    def connect(self):
        self.conn.connect()

    def disconnect(self):
        self.conn.disconnect()

    def ping(self):
        resp = self.conn.send("ping")
        if "PONG" not in resp:
            raise AssertionError(f"Expected PONG, got: {resp}")
        return resp

    def help(self):
        return self.conn.send("help")

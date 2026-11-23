import serial
import time
import yaml

class TestbenchLibrary:
    def __init__(self, config_path="config/testbench.yaml"):
        with open(config_path) as f:
            cfg = yaml.safe_load(f)["testbench"]
        self.port = cfg["port"]
        self.baud = cfg["baudrate"]
        self.timeout = cfg["timeout"]
        self.ser = None

    def connect(self):
        self.ser = serial.Serial(self.port, self.baud, timeout=self.timeout)
        time.sleep(2)
        self.ser.reset_input_buffer()

    def disconnect(self):
        if self.ser and self.ser.is_open:
            self.ser.close()

    def send_command(self, cmd):
        self.ser.write((cmd + "\r\n").encode())
        self.ser.flush()
        lines = []
        deadline = time.time() + self.timeout
        while time.time() < deadline:
            raw = self.ser.readline()
            if raw:
                text = raw.decode(errors="replace").strip()
                if text:
                    lines.append(text)
                    if text.startswith("OK") or text.startswith("ERR") or text.startswith("VAL") or text.startswith("PONG"):
                        break
        return "\n".join(lines)

    def expect_ok(self, cmd):
        resp = self.send_command(cmd)
        if "OK" not in resp:
            raise AssertionError(f"Expected OK for '{cmd}', got: {resp}")
        return resp

    def expect_val(self, cmd, expected):
        resp = self.send_command(cmd)
        if f"VAL {expected}" not in resp:
            raise AssertionError(f"Expected 'VAL {expected}' for '{cmd}', got: {resp}")
        return resp

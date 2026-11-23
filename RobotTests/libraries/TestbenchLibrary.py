import serial
import time
import yaml

class TestbenchLibrary:
    def __init__(self, config_path="${CURDIR}/../config/testbench.yaml"):
        # resolve relative to this file if needed
        import os
        if not os.path.isabs(config_path) and not os.path.exists(config_path):
            base = os.path.dirname(os.path.abspath(__file__))
            config_path = os.path.join(base, "..", "config", "testbench.yaml")
        with open(config_path) as f:
            cfg = yaml.safe_load(f)["testbench"]
        self.port = cfg["port"]
        self.baud = cfg["baudrate"]
        self.timeout = cfg["timeout"]
        self.prompt = cfg.get("prompt", "tb:~$")
        self.ser = None

    def connect(self):
        self.ser = serial.Serial(self.port, self.baud, timeout=self.timeout)
        time.sleep(2)
        self.ser.reset_input_buffer()
        self._read_until_prompt()

    def disconnect(self):
        if self.ser and self.ser.is_open:
            self.ser.close()

    def _read_until_prompt(self):
        buf = ""
        deadline = time.time() + self.timeout
        while time.time() < deadline:
            raw = self.ser.read(self.ser.in_waiting or 1)
            if raw:
                buf += raw.decode(errors="replace")
                if self.prompt in buf:
                    break
        return buf

    def send_command(self, cmd):
        self.ser.write((cmd + "\n").encode())
        self.ser.flush()
        buf = self._read_until_prompt()
        lines = [l.strip() for l in buf.splitlines() if l.strip()]
        out = []
        for l in lines:
            if l == cmd.strip():
                continue
            if self.prompt in l:
                l = l.replace(self.prompt, "").strip()
            if l:
                out.append(l)
        return "\n".join(out)

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

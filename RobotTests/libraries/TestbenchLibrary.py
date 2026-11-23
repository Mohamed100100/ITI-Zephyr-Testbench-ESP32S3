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
        self.prompt = cfg.get("prompt", "tb:~$")

    def connect(self):
        self.ser = serial.Serial(self.port, self.baud, timeout=self.timeout)
        time.sleep(2)
        self.ser.reset_input_buffer()
        # wait for prompt
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
        # remove the echoed command and prompt
        lines = [l.strip() for l in buf.splitlines() if l.strip()]
        # drop echo of command and trailing prompt line
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

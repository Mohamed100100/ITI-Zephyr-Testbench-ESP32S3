import os
import time
import serial
import yaml


def load_config(config_path="config.yaml"):
    if not os.path.isabs(config_path) and not os.path.exists(config_path):
        base = os.path.dirname(os.path.abspath(__file__))
        config_path = os.path.join(base, "..", "config", "testbench.yaml")
    # fallback name
    if not os.path.exists(config_path):
        base = os.path.dirname(os.path.abspath(__file__))
        config_path = os.path.join(base, "..", "..", "config", "testbench.yaml")
    with open(config_path) as f:
        return yaml.safe_load(f)["testbench"]


class SerialConnection:
    def __init__(self, config_path=None):
        cfg = load_config(config_path) if config_path else load_config()
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
            self.ser = None

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

    def send(self, cmd):
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

#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess, time, serial    # pip install pyserial

cmd_btn_wind = """powershell -c "$wshell = New-Object -ComObject wscript.shell; $wshell.SendKeys('^{ESCAPE}')"""
cmd_btn_mute = """powershell (new-object -com wscript.shell).SendKeys([char]173)"""

if __name__ == "__main__":
    comport = serial.Serial(port='//./COM4', baudrate=115200)

    while True:
        try:
            line = comport.readline().strip().decode()
            print(line)

            if line == "idle":              # skip idle messages
                continue
            elif line == "on":
                subprocess.Popen(cmd_btn_wind, close_fds=True)
            elif line == "off":
                subprocess.Popen(cmd_btn_mute, close_fds=True)
            else:
                print(F"unknown '{line}'")
                continue                    # skip if incomplete command

            time.sleep(0.3)                 # rate limit
            flush = comport.read_all()      # flush inbound buffer

        except KeyboardInterrupt:
            break

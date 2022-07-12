#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess, time, serial    # pip install pyserial

if __name__ == "__main__":
    comport = serial.Serial(port='//./COM4', baudrate=115200)

    while True:
        try:
            line = comport.readline().strip().decode()
            print(line)
            if line == "idle":              # skip idle messages
                continue

            pin, value = line.split(':')    # expect line to be like   2:0  or 2:1

            if value == '1':
                subprocess.Popen("notepad.exe", close_fds=True)
            if value == '0':
                subprocess.Popen("taskkill /im notepad.exe")

            time.sleep(1)
            flush = comport.read_all()  # flush any buffer
        except KeyboardInterrupt:
            break

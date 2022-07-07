#!/usr/bin/env python
# -*- coding: utf-8 -*-

import serial   # pip install pyserial
from pprint import pprint

if __name__ == "__main__":
    comport = serial.Serial(port='//./COM5', baudrate=115200)

    while True:
        line = comport.readline().decode().strip()
        data = {}
        try:
            for word in line.split(' '):      # split line by blanc
                key, value = word.split(':')  # split word by ':'
                data[key] = int(value)

            pprint(data)

        except Exception as e:
            print(F"WARN: could not parse line '{line}'")
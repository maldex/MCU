#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys

import serial   # pip install pyserial
import threading
import time

class ClauStick(threading.Thread):
    def __init__(self, serial_port='//./COM5', baud_rate=115200):
        super().__init__()  # construct thread
        self._running = False
        self.serial_port, self.baud_rate = serial_port, baud_rate


    def run(self):
        self._running = True
        try:
            self.comport = serial.Serial(port=self.serial_port, baudrate=self.baud_rate)
        except Exception as e:
            raise(e)
            self.stop()

        while self._running:
            self.read_from_serial()

    def stop(self):
        self._running = False

    def read_from_serial(self):
        try:
            line = self.comport.readline().decode().strip()
            self.values = {}
            for word in line.split(' '):
                print(word.strip().split(':'))
                # self.values[key] = value
        except Exception as e:
            print("ERROR READING COMPORT " + str(e))

if __name__ == "__main__":
    try:
        print("hi there")

        MyStick = ClauStick()
        MyStick.start()

        while True:
            print("main thread")
            time.sleep(4)


    except KeyboardInterrupt as e:
        MyStick.stop()
        raise(e)

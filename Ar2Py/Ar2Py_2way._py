#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Sinmple example how to communicate between arduino and python
#
# requires pyserial  "pip install pyserial"

import os, serial, logging, threading, subprocess
from time import sleep
from pprint import pprint


class SerialReader(threading.Thread):
    def __init__(self, port="'//./COM3", baudrate=115200):
        super().__init__()
        logging.info(F"opening '{port}' at '{baudrate}")
        self.ser = serial.Serial(port='//./COM3', baudrate=115200)
        self.buffer = []

    def run(self):  # thread
        self.running = True
        logging.info(F"Starting reader thread")
        while self.running:
            line = self.ser.readline().decode('ascii').strip()
            logging.debug(F"read line '{line}'")
            self.buffer.append(line)
        logging.info(F"Stopping reader thread")

    def stop(self):
        self.running = False
        logging.info(F"closing serial port")
        self.ser.close()

    def available(self):
        return len(self.buffer) != 0

    def read(self):
        return self.buffer.pop()

    def write(self, line):
        logging.debug(F"write line '{line}'")
        line += '\n'
        self.ser.write(line.encode('ascii'))


if __name__ == "__main__":
    logging.basicConfig(format='%(levelname)s:%(message)s', level=logging.DEBUG)

    Arduino = SerialReader()
    Arduino.start()

    ledstate = False

    try:
        while True:
            while Arduino.available():
                if Arduino.read() == "2:1":
                    while Arduino.available(): Arduino.read()  # drain queue = simple debouncing
                    print("------ TRIGGER ---------")

                    subprocess.Popen("notepad.exe", close_fds=True)



            # blink LED
            Arduino.write(F"LED {ledstate}")
            ledstate = not ledstate

            sleep(1)

    except KeyboardInterrupt:
        Arduino.stop()

#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
pls_help = '''  To whoever reads this: please help me re-write this in C.

Although generating PWM using the GPIO does work this way, python's accuracy
is around 0.01sec / 100Hz, which is not enough to operate led or servo motors
in a smooth way. Python is not linear in it's execution time, so there is a 
slight variation when you try to do near-real-time stuff.

So for what i'd like to do:
- arg-parsing (--pin X, --time Y, etc)
- initializing the gpio (*) (writing bytes into file-like object)
- looping for the specified time (blocking)
-- set pin high (**) (writing "1" into /sys/class/gpio/gpioX/value)
-- delay some micro-seconds
-- set pin low (writing "0")
-- delay some frequency - micro-seconds
- de-initializing the gpio ...

(*) For the APU/NCT5104d GPIO headers, enabling and high/low the GPIO pin is 
quite easy though writing bytes into the /sys/class/gpio. 

(**) As this is usually also the signal-rise for electronic components, it's
quite crucial this happens as timely as possible, the more accurate the better
Hence the idea to implement this in plain C.

cheers
'''


program_description = '''Software Pulse Width Modulation for GPIO Pins.

You remember PWM? For instance, your GPIO generates 0 or 5Volt

    25% dutycycle       50% dutycyle         75% dutycyle     ....
 +-+   +-+   +-+      +--+  +--+  +--+     +---+ +---+ +---+
 | |   | |   | |      |  |  |  |  |  |     |   | |   | |   |
-+ +---+ +---+ +---  -+  +--+  +--+  +--  -+   +-+   +-+   +-
     ~1.25Volt           ~2.5Volt            ~3.75Volt        ....


example: apply 3Volt to GPIO Pin 3 for 2 seconds
./SoftPWM.py --pin 3 --percent 60 --duration 2
'''

from multiprocessing import Process, Value
from time import sleep, time
from abc import ABCMeta, abstractmethod

class _PWM(object):
    # abstract generic PWM generating class, please implement __setPin() method
    __metaclass__ = ABCMeta
    def __init__(self, pin, freq=0.01):
        assert isinstance(pin, int)
        assert isinstance(freq, float)
        self.pin = pin
        self.__freq = freq
        self.__duty_cycle = 0
        self.__thread = None
        self.__timeout = Value('f', 0)  # thread-save data-type

    def is_attached(self):
        return (not self.__thread is None)

    def attach(self):
        # start oscillator-thread
        if self.is_attached():
            self.detach()
        self.__thread = Process(target=self.__oscillate, args=())
        self.__thread.daemon = True
        self.__thread.start()

    def detach(self):
        # stop oscillator-thread
        if not self.is_attached():
            return
        self.__timeout.value = time()      # syncrhonized
        self.__thread.join()
        self.__thread = None

    def __oscillate(self):
        # here the magic is happening: on-off-on-off-on-off
        time_on = self.__duty_cycle
        time_off = self.__freq - self.__duty_cycle
        # todo: improve the PWM generation - review timing
        while self.__timeout.value == 0 or time() < self.__timeout.value:
            self._setPin(True)             # set pin on
            sleep(time_on)
            self._setPin(False)            # set pin off
            sleep(time_off)

    def setPercent(self, percent, duration=None):
        # user-access: set duty-cycle in percent
        assert isinstance(percent, float)
        assert (isinstance(duration, float) or isinstance(duration, type(None)))
        self.detach()
        self.__duty_cycle = self.__freq * percent / 100.0
        self.attach()
        if not duration is None:
            # as __timeout is synchronized, this can be done after thread-start
            assert duration > 0.0
            self.__timeout.value = time() + duration

    def __del__(self):
        try:
            self.detach()
        except Exception,e:
            pass

    @abstractmethod
    def _setPin(self, value):    # this method must be defined by inheritors
        pass


# https://sourceforge.net/p/raspberry-gpio-python/wiki/BasicUsage/
class PWM_RPi(_PWM):
    # PWM Class for Raspberry Pi
    def __init__(self, *args, **kwargs):
        super(type(self), self).__init__(*args, **kwargs)
        import RPi.GPIO
        self.GPIO = RPi.GPIO
        self.GPIO.setmode(self.GPIO.BCM)
        self.GPIO.setup(self.pin, self.GPIO.OUT)
        self._setPin(False)

    def _setPin(self, value):
        self.GPIO.output(self.pin, value)

    def __del__(self):
        super(type(self), self).__del__()
        try:
            self.GPIO.cleanup(self.pin)
        except Exception,e:
           pass

# https://github.com/pcengines/linux-gpio-nct5104d
class PWM_APU(_PWM):
    def __init__(self, *args, **kwargs):
        super(type(self), self).__init__(*args, **kwargs)
        self.__export_file = "/sys/class/gpio/export"
        self.__unexport_file = "/sys/class/gpio/unexport"
        self.__direction_file = "/sys/class/gpio/gpio" + str(self.pin) + "/direction"
        self.__value_file = "/sys/class/gpio/gpio" + str(self.pin) + "/value"
        try:
            with open(self.__export_file, 'w') as f:
                f.write(str(self.pin))
        except IOError, e:
            print "cannot open pin "+ str(self.pin) + ", maybe unexport with 'echo "+str(self.pin) +" > /sys/class/gpio/unexport"
            quit(4)
        with open(self.__direction_file, 'w') as f:
            f.write("out")
        self._setPin(False)

    def _setPin(self, value):
        with open(self.__value_file, 'w') as f:
            f.write(str(int(value)))

    def __del__(self):
        super(type(self), self).__del__()
        try:
            with open(self.__unexport_file, 'w') as f:
                f.write(str(self.pin))
        except Exception,e:
            pass

# here we actually go
if __name__ == "__main__":
    from argparse import ArgumentParser, RawTextHelpFormatter
    from os import path

    ### guessing the right platform, don't try this at home
    PWM_Class = None

    if path.exists("/sys/class/gpio"):  # assume APU
        PWM_Class = PWM_APU

    try:
        import RPi.GPIO                 # assume RasPi
        PWM_Class = PWM_RPi
    except Exception, e:
        pass

    if PWM_Class == None:
        print "only APU/NCT5104d or Raspberry Pi supported"
        quit(2)

    ### runtime arg parsing
    parser = ArgumentParser(description=program_description, formatter_class=RawTextHelpFormatter)
    parser.add_argument('-p', '--pin', action='store', default=None, dest='pin',
                        help='physical connector pin on GPIO')
    parser.add_argument('-e', '--percent', action='store', default=50, dest='percent',
                        help='dutycycle in percent (0.0 - 100.0) [%(default)s]')
    parser.add_argument('-d', '--duration', action='store', default=1, dest='duration',
                        help='how long to apply PWM')
    parser.add_argument('-f', '--freq', action='store', default=0.01, dest='freq',
                        help='1/frequency of the oscillator [%(default)s]')
    parser.add_help
    args = parser.parse_args()  # command line arg parser

    try:
        pin = int(args.pin)
        percent = float(args.percent)
        duration = float(args.duration)
        freq = float(args.freq)
    except TypeError, e:
        print e.message
        print "see --help"
        quit(1)
    if percent <= 0.0 or percent > 100.0:
        print "expecting 0 < duration <= 100.0"
        quit(1)

    ### here we go

    # instantiate the previously figured PWM-Child-Class
    MyPWM = PWM_Class(pin, freq=freq)

    # start PWM generating thread
    MyPWM.setPercent(percent)

    # sleep a bit
    sleep(duration)

    # stop PWM generation again
    MyPWM.detach()

    # all done
    quit(0)
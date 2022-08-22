# Dummy Electronic CountDown timer for explosive charge
Movie Project that requires a bomb (clay) and a 'modern' controller, featuring a digital display and numeric keypad. Display shows 30 seconds steady, actor enters the wrong number which activates the timer.

### BOM
- [Arduino uno](https://docs.arduino.cc/hardware/uno-rev3) (logic)
- [Adafruit 4-Digit 7-Segment Display](https://www.adafruit.com/product/879) (timer output)
- [tactile buttons](https://www.adafruit.com/product/367) (keypad input)
- 9V Battery (supply)
- wires, perfboard, etc

### wiring
| Arduino Pin | Component |
|---|---|
| GND  | GND of buttons and display  |
| 3.3V  | positive/resistor supply for buttons  |
| 5V  | powersupply of display  |
| Vin  | positive battery  |
| A0 | button top right (3)  |
| A1 | button top left (1)  |
| A2 | button bottom right (#)  |
| A3 | button bottom left (*)  |
| A4/SDA  | I2C to display (D)  |
| A5/SCL  | I2C to display (C)  |
| D13 | led (arduino onboard)  |

### State-diagram




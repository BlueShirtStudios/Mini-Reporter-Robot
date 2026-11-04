from machine import Pin, ADC
from utime import sleep

xAxis = ADC(1)
yAxis = ADC(0)
joystickButton = Pin(22, Pin.IN, Pin.PULL_UP)

def map_range(value, in_min, in_max, out_min, out_max):
    return int((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

while True:
    try:
        x = xAxis.read_u16()
        y = yAxis.read_u16();
        
        servo_duty = map_range(x, 0, 65535, 1638, 8192)
        
        print(f"{x}, {y}, {servo_duty}")
       
        sleep(0.5)
        
    except KeyboardInterrupt:
        break


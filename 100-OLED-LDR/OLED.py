from machine import Pin, I2C, PWM, ADC
import ssd1306
from time import sleep

ANALOG_PIN = 4
detector = ADC(Pin(ANALOG_PIN))
detector.atten(ADC.ATTN_11DB)

i2c = I2C(sda=Pin(48), scl=Pin(47))
display = ssd1306.SSD1306_I2C(128, 64, i2c)
display.text('Light Level:', 0, 0, 1)
display.rect(10, 15, 87, 23, 1)
                
while True:
    display.fill(0)
    bar = 100-int((detector.read())/40.95)
    x = int((87/100)*bar)
    display.text('Light Level:', 0, 0, 1)
    display.rect(10, 15, 87, 23, 1)
    display.fill_rect(10, 15, x, 23, 1)
    display.text(f"{bar} ", 105, 23, 1)
    display.show()

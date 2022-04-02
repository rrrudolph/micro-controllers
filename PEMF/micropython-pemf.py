from machine import Pin
import time
import random


# This code is meant specifically for the RP2040 chip
# which is found in the RPi Pico. Can't confirm if this
# code would run on other micropython boards


DUTY_CYCLE = 0.15  # 15%
SIGNAL = Pin(14, Pin.OUT) 


# 
def _get_random_values(frequency: int, order='descend'):
    """Append random decimal values to each whole number for variety"""

    random.seed()
    a = frequency + random.uniform(0.75, 0.99)
    b = frequency + random.uniform(0.5, 0.75)
    c = frequency + random.uniform(0.25, 0.5)
    d = frequency + random.uniform(0, 0.25)

    return a, b, c, d if order == 'descend' else d, c, b, a

    
def _generate_frequency(frequency: float, duration: int):
    """Duration needs to be in milliseconds"""
    
    # convert frequency from hz to milliseconds
    freq = frequency / 1000
    
    # get on / off times
    signal_on = freq * DUTY_CYCLE * 65025 # built in max value
    signal_off = freq - signal_on
    
    end = time.ticks_ms() + duration
    
    while time.ticks_ms() < end:
        SIGNAL.on()
        time.sleep_ms(signal_on)
        SIGNAL.off()
        time.sleep_ms(signal_off)
        
    
def run_frequencies(start, stop, minutes):
    
    total_milliseconds = minutes * 60 * 1000
    total_frequencies = abs(start - stop)
    millis_per_freq = total_milliseconds / total_frequencies / 4

    # descend
    if start > stop:
        for frequency in range(start, stop - 1, -1):
            frequencies = _get_random_values(frequency)
            for f in frequencies:
                _generate_frequency(f, millis_per_freq)
    
    # ascend
    if start < stop:
        for frequency in range(start, stop + 1):
            frequencies = _get_random_values(frequency, order='ascend')
            for f in frequencies:
                _generate_frequency(f, millis_per_freq)
            

# this will generate 4 times the amount of frequencies that 
# are passed in and they will be random values with each run
run_frequencies(21, 1, 300)
        
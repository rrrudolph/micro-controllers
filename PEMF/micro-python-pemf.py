from machine import Pin
import utime
import random

# built in duty cycle values are 0 to 65025 so
# need a conversion function to map 100% to 65025
def get_duty_cycle(percentage):
    # pass in a decimal, like 0.20 for 20%
    return percentage * 65025

# for each whole number frequency step, I want to choose
# 3 random decimal values within that number to add
# random variety to the frequencies that are generated
def get_random_decimals(integer, order=None):
    # I guess for micropython, random.seed() gives you a way
    # to provide deterministic randomness but is not necessary
    # for random randomness
    if order == 'descend':
        # order by descending values
        a = random.uniform(0.66, 0.99)
        b = random.uniform(0.33, 0.66)
        c = random.uniform(0, 0.33)
    
    else:
        # order by ascending
        a = random.uniform(0, 0.33)
        b = random.uniform(0.33, 0.66)
        c = random.uniform(0.66, 0.99)
    
    return a, b, c

def generate_frequencies(start, stop, minutes):
    
    total_time = minutes * 60  # convert to seconds
    total_frequencies = abs(start - stop)
    time_per_freq = total_time / total_frequencies / 3
    
    # ascend or descend based on order of frequencies passed
    
    # ascend
    if start < stop:
        for i in range(start, stop):
            
            # generate the decimal values for this freq
            a, b, c = get_random_decimals(i, 'ascend')
            freq_1 = i + a
            freq_2 = i + b
            freq_3 = i = c
            
            
    
signal = Pin(15, Pin.OUT)

duty_cycle = get_duty_cycle(0.15)


        
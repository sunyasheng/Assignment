#!/usr/bin/env python3
import sys
current_city_name = None
city_name = None
current_city_highest_temperature = -1e9
current_city_lowest_temperature = 1e9

for line in sys.stdin:
    line = line.strip()
    city_name, city_temperature = line.split('\t', 1)
    try:
        city_temperature = float(city_temperature)
    except:
        continue
    if current_city_name == city_name:
        current_city_highest_temperature = max(city_temperature, current_city_highest_temperature)
        current_city_lowest_temperature = min(city_temperature, current_city_lowest_temperature)
    else:
        if current_city_name:
            print('h\t{0}\t{1}'.format(current_city_name, current_city_highest_temperature))
            print('l\t{0}\t{1}'.format(current_city_name, current_city_lowest_temperature))
        current_city_highest_temperature = city_temperature
        current_city_lowest_temperature = city_temperature
        current_city_name = city_name

if city_name == current_city_name:
    print('h\t{0}\t{1}'.format(current_city_name, current_city_highest_temperature))
    print('l\t{0}\t{1}'.format(current_city_name, current_city_lowest_temperature))
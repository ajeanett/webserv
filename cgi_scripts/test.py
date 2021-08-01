#!/usr/local/bin/python3


import random
import datetime

print('Content-Type: text/plain', end="\r\n")
print('', end="\r\n")

print('It\'s Python! (test.py)')
print()
print(f'Random number: {random.randint(0, 1000)}')
print(f'time: {datetime.datetime.now()}')

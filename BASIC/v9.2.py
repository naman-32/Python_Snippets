import sys

sys.path.append('/D:/DIVYA')
from my_module import find_index as fi,test
#import my_module  as mm write mymodule.findindex

courses = ['math','science','phy','chem']
print(fi(courses,'science'))

print(test)

import random
print(random.choice(courses))

import math
print(math.radians(180))
print(math.sin(math.radians(180)))

import datetime
import calendar
print(calendar.isleap(2020))
print(datetime.date.today())

import os
print(os.getcwd())
print(os.__file__)
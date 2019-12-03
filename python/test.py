# -*- coding: utf-8 -*-
"""
Created on Tue Aug  6 16:38:48 2019

@author: SWTestPC
"""

import cv2
import numpy as np

a = 250*np.ones((2, 4, 3), dtype = 'uint8')
print(a)
print(a.shape)
print(cv2.add(a, 10))

b = np.zeros((2, 4, 3), dtype = 'uint8')

c = a/b
c[c==np.inf] = 0
print(c)
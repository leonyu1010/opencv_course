# -*- coding: utf-8 -*-
"""
Created on Tue Jul 30 09:15:51 2019

@author: SWTestPC
"""

import cv2
import numpy as np
import matplotlib.pyplot as plt
import matplotlib

matplotlib.rcParams['figure.figsize'] = (6.0, 6.0)
matplotlib.rcParams['image.cmap'] = 'gray'

im = np.zeros((10, 10), dtype='uint8')
print(im)
plt.imshow(im)

im[0, 1] = 1
im[-1, 0] = 1
im[-2, -1] = 1
im[2, 2] = 1
im[5:8, 5:8] = 1

print(im)
plt.imshow(im)

element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3, 3))
print(element)
ksize = element.shape[0]
height, width = im.shape[:2]
print(im.shape)
print(element.shape)

dilated = cv2.dilate(im, element)
print(dilated)
plt.imshow(dilated)

border = ksize // 2
print(border)

paddedIm = np.zeros((height + border*2, width + border*2))
paddedIm = cv2.copyMakeBorder(im, border, border, border, border,
                              cv2.BORDER_CONSTANT, value=0)

paddedDilatedIm = paddedIm.copy()
plt.imshow(im)
plt.imshow(paddedIm)

dilationAVI = cv2.VideoWriter('dilationScratch.avi',
                              cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'),
                              10, (50, 50))

for h_i in range(border, height+border):
    for w_i in range(border, width+border):
        and_result = cv2.bitwise_and(paddedIm[h_i-border: h_i+border+1,
                                              w_i-border: w_i+border+1],
                                     element)
        paddedDilatedIm[h_i, w_i] = and_result.max()
        resizedFrame = cv2.resize(paddedDilatedIm, (50, 50))
        bgrFrame = cv2.cvtColor(resizedFrame * 255, cv2.COLOR_GRAY2BGR)

        dilationAVI.write(bgrFrame)

dilationAVI.release()

cropped = paddedDilatedIm[border:border+height, border:border+width]

plt.imshow(cropped)


Eroded = cv2.erode(im, element)
print(Eroded)
plt.imshow(Eroded)

border = ksize // 2
print(element)

paddedIm = np.zeros((height + border*2, width + border*2))
paddedIm = cv2.copyMakeBorder(im, border, border, border, border,
                              cv2.BORDER_CONSTANT, value=1)

paddedErodedIm = paddedIm.copy()
plt.imshow(im)
plt.imshow(paddedErodedIm)


erosionAVI = cv2.VideoWriter('erosionScratch.avi',
                              cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'),
                              10, (50, 50))

sumOfElement = element.sum()
print(sumOfElement)

for h_i in range(border, height+border):
    for w_i in range(border, width+border):
        and_result = cv2.bitwise_and(paddedIm[h_i-border: h_i+border+1,
                                              w_i-border: w_i+border+1],
                                     element)
        s = and_result.sum()
        if(s == sumOfElement):
            paddedErodedIm[h_i, w_i] = 1
        else:
            paddedErodedIm[h_i, w_i] = 0

        resizedFrame = cv2.resize(paddedErodedIm, (50, 50))
        bgrFrame = cv2.cvtColor(resizedFrame * 255, cv2.COLOR_GRAY2BGR)

        erosionAVI.write(bgrFrame)

erosionAVI.release()
cropped = paddedErodedIm[border:border+height, border:border+width]

plt.imshow(cropped)


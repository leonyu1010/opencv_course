# -*- coding: utf-8 -*-
"""
Created on Tue Aug 13 14:48:19 2019

@author: SWTestPC
"""
import sys
import cv2
import matplotlib.pyplot as plt
import numpy as np
import ImagePath
import matplotlib
import math
import pprint
import timeit

np.set_printoptions(threshold=sys.maxsize)

matplotlib.rcParams['figure.figsize'] = (18.0, 12.0)
matplotlib.rcParams['image.interpolation'] = 'bilinear'

image_file = "sample.jpg"

image = cv2.imread(ImagePath.get_image_path(image_file), 1)
image_clone = image.copy()

plt.imshow(image[:,:,::-1])
plt.show()

def convertBGRtoGray(image):
    ###
    ### YOUR CODE HERE
    ###
    b = np.float32(image[:,:,0])/255.0
    g = np.float32(image[:,:,1])/255.0
    r = np.float32(image[:,:,2])/255.0
    gray = (0.299*r + 0.587*g + 0.114*b)*255
    return np.around(gray).astype('int16')


gray = convertBGRtoGray(image)
print("gray", gray.dtype, gray.min(), gray.max())
gray_cv = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
print("gray_cv", gray_cv.dtype, gray_cv.min(), gray_cv.max())
diff = np.abs(gray - gray_cv)
maxVal = np.amax(diff)
print(maxVal)

plt.figure(figsize=(18,12))
plt.subplot(1,3,1)
plt.title("result from function")
plt.imshow(gray, cmap="gray")
plt.subplot(1,3,2)
plt.title("result from cv")
plt.imshow(gray_cv, cmap='gray')
plt.subplot(1,3,3)
plt.title("diff")
plt.imshow(np.abs(gray-gray_cv), cmap="gray")
plt.show()

def getHSV(b, g, r):
    bg_max = np.maximum(b, g)
    max_val = np.maximum(bg_max, r)
    bg_min = np.minimum(b, g)
    min_val = np.minimum(bg_min, r)
    delta = max_val - min_val

    v = max_val
    s = 0.0
    if(max_val > 0):
        s = delta / max_val

    h = 0.0
    if(max_val == min_val):
        h = 0.
    else:
        if(max_val == r):
            h = 60. * (g - b) / delta
        elif (max_val == g):
            h = 120. + 60. * (b - r) / delta
        elif (max_val == b):
            h = 240. + 60. * (r - g) / delta

    if(h < 0):
        h = h + 360.
    return h/2., s*255., v*255.

def convertBGRtoHSV_V(image):
    ###
    ### YOUR CODE HERE
    ###
    img = image.astype(np.float32) / 255.
    b, g, r = cv2.split(img)
    getfunc = np.vectorize(getHSV)
    h, s, v = getfunc(b, g, r)
    hsv = cv2.merge((h, s, v))
    return (np.around(hsv)).astype('int16')


def convertBGRtoHSV(image):
    ###
    ### YOUR CODE HERE
    ###
    img = image.astype(np.float32) / 255.
    hsv = np.zeros_like(img)

    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            b, g, r = img[i,j]
            max_val = max(img[i, j])
            min_val = min(img[i, j])
            delta = max_val - min_val

            v = max_val

            s = 0.0
            if(max_val > 0):
                s = delta / max_val

            h = 0.0
            if(delta == 0.0):
                h = 0.0
            else:
                if(max_val == r):
                    h = 60. * (g - b) / delta
                elif (max_val == g):
                    h = 120. + 60. * (b - r) / delta
                elif (max_val == b):
                    h = 240. + 60. * (r - g) / delta
                else:
                    print("wrong")

            if(h < 0):
                h = h + 360.

            hsv[i,j] = (h/2., s*255., v*255.)

    return (hsv+0.5).astype(np.int16)

hsv= convertBGRtoHSV_V(image)
#print(timeit.timeit("convertBGRtoHSV(image)", number = 100000))
#print(timeit.timeit('convertBGRtoHSV_V(image)', number = 100000))

hsv_cv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
diff = np.abs(hsv.astype(np.float32)-hsv_cv.astype(np.float32))
diff[diff==180] = 0

maxVal = np.amax(diff)
print(maxVal)
plt.figure(figsize=(18,12))
plt.subplot(1,3,1)
plt.title("result from function")
plt.imshow(hsv[:,:,::-1])
plt.subplot(1,3,2)
plt.title("result from cv")
plt.imshow(hsv_cv[:,:,::-1])
plt.subplot(1,3,3)
plt.title("diff")
plt.imshow(np.abs(hsv-hsv_cv)[:,:,::-1])
plt.show()

#print(np.nonzero(hsv - hsv_cv))

# -*- coding: utf-8 -*-
"""
Created on Mon Aug  5 14:06:51 2019

@author: SWTestPC
"""

import cv2
import matplotlib.pyplot as plt
import numpy as np
import ImagePath
import matplotlib
import random as rng

matplotlib.rcParams['figure.figsize'] = (10.0, 10.0)
matplotlib.rcParams['image.cmap'] = 'gray'

def boundingBox(src_gray):
    contours, hierarchy = cv2.findContours(src_gray, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)


    contours_poly = [None]*len(contours)
    boundRect = [None]*len(contours)
    centers = [None]*len(contours)
    radius = [None]*len(contours)
    for i, c in enumerate(contours):
        contours_poly[i] = cv2.approxPolyDP(c, 3, True)
        boundRect[i] = cv2.boundingRect(contours_poly[i])
        centers[i], radius[i] = cv2.minEnclosingCircle(contours_poly[i])


    drawing = np.zeros((src_gray.shape[0], src_gray.shape[1], 3), dtype=np.uint8)


    for i in range(len(contours)):
        color = (rng.randint(0,256), rng.randint(0,256), rng.randint(0,256))
        cv2.drawContours(drawing, contours_poly, i, color)
        cv2.rectangle(drawing, (int(boundRect[i][0]), int(boundRect[i][1])), \
          (int(boundRect[i][0]+boundRect[i][2]), int(boundRect[i][1]+boundRect[i][3])), color, 2)
        #cv2.circle(drawing, (int(centers[i][0]), int(centers[i][1])), int(radius[i]), color, 2)


    plt.figure()
    plt.imshow(drawing[:,:,::-1])
    plt.title("boundingBox")

image_file = "car_detect.png"

image = cv2.imread(ImagePath.get_image_path(image_file), 1)
image_clone = image.copy()
plt.figure()
plt.imshow(image[:,:,::-1]);
plt.title("Original Image")

image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
plt.figure()
plt.imshow(image_gray);
plt.title("Gray")

image_b, image_g, image_r = cv2.split(image)
plt.figure(figsize=(20,12))
plt.subplot(141)
plt.imshow(image[:,:,::-1]);
plt.title("Original Image")
plt.subplot(142)
plt.imshow(image_b);
plt.title("Blue Channel")
plt.subplot(143)
plt.imshow(image_g);
plt.title("Green Channel")
plt.subplot(144)
plt.imshow(image_r);
plt.title("Red Channel");

_, image_binary = cv2.threshold(image_b, 127, 255, cv2.THRESH_BINARY_INV)
plt.figure()
plt.imshow(image_binary);
plt.title("image_binary")

element = cv2.getStructuringElement(cv2.MORPH_RECT,(11, 11))


image_gradient = cv2.morphologyEx(image_binary, cv2.MORPH_GRADIENT, element)
plt.figure()
plt.imshow(image_gradient);
plt.title("image_gradient")

#image_binary = image_gradient # cv2.bitwise_not(image_eroded)

image_dilated = cv2.dilate(image_binary, element, iterations=2)
plt.figure()
plt.imshow(image_dilated);
plt.title("image_dilated")

image_eroded = cv2.erode(image_dilated, element, iterations=2)
plt.figure()
plt.imshow(image_eroded);
plt.title("image_eroded")

image_binary = image_eroded# cv2.bitwise_not(image_eroded)
plt.figure()
plt.imshow(image_binary);
plt.title("image_binary")
contours, hierarchy = cv2.findContours(image_binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
print("Number of contours found = {}".format(len(contours)))
image = image_clone.copy()
cv2.drawContours(image, contours, -1, (255,255,0), 3);
plt.figure()
plt.imshow(image[:,:,::-1])
plt.title("external contours")

boundingBox(image_binary)

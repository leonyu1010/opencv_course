# -*- coding: utf-8 -*-
"""
Created on Fri Aug  2 15:44:18 2019

@author: SWTestPC
"""

import cv2
import matplotlib.pyplot as plt
import numpy as np

import matplotlib
matplotlib.rcParams['figure.figsize'] = (10.0, 10.0)
matplotlib.rcParams['image.cmap'] = 'gray'

image = cv2.imread('CoinsA.png', 1)
imageCopy = image.copy()

imageGray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

plt.figure(figsize=(12,12))
plt.subplot(121)
plt.imshow(image[:,:,::-1]);
plt.title("Original Image")
plt.subplot(122)
plt.imshow(imageGray);
plt.title("Grayscale Image");

imageB, imageG, imageR = cv2.split(image)



_, imBinary = cv2.threshold(imageG, 60, 255, cv2.THRESH_BINARY)
_, imThresh20 = cv2.threshold(imageG, 20, 255, cv2.THRESH_BINARY_INV)
_, imThresh30 = cv2.threshold(imageG, 30, 255, cv2.THRESH_BINARY_INV)
_, imThresh50 = cv2.threshold(imageG, 50, 255, cv2.THRESH_BINARY_INV)

plt.figure(figsize=(60,60))
plt.subplot(141)
plt.imshow(imBinary)
plt.title("imBinary")
plt.subplot(142)
plt.imshow(imThresh20)
plt.title("imThresh20")
plt.subplot(143)
plt.imshow(imThresh30)
plt.title("imThresh30")
plt.subplot(144)
plt.imshow(imThresh50)
plt.title("imThresh50")




dilations = []

for i in [3, 5, 7, 9]:
    imThreshold = imThresh30.copy()
    ksize = i
    element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (i,i))
    dilation = cv2.dilate(imThreshold, element)
    dilations.append(dilation)

plt.figure(figsize=(60, 60))
plt.subplot(161)
plt.imshow(dilations[0]);
plt.title("dilations size 3")
plt.subplot(162)
plt.imshow(dilations[1]);
plt.title("dilations size 5")
plt.subplot(163)
plt.imshow(dilations[2]);
plt.title("dilations size 7")
plt.subplot(164)
plt.imshow(dilations[3]);
plt.title("dilations size 9");


imDilated = dilations[2]
plt.imshow(imDilated);
plt.title("imDilated")

erosions = []

for i in [3, 5, 7, 9]:
    ksize = i
    element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (i,i))
    erosion = cv2.erode(imDilated, element)
    erosions.append(erosion)

plt.figure(figsize=(60, 60))
plt.subplot(171)
plt.imshow(erosions[0]);
plt.title("erosions size 3")
plt.subplot(172)
plt.imshow(erosions[1]);
plt.title("erosions size 5")
plt.subplot(173)
plt.imshow(erosions[2]);
plt.title("erosions size 7")
plt.subplot(174)
plt.imshow(erosions[3]);
plt.title("erosions size 9");

imEroded = erosions[3]

# Set up the SimpleBlobdetector with default parameters.
params = cv2.SimpleBlobDetector_Params()

params.blobColor = 0

params.minDistBetweenBlobs = 2

# Filter by Area.
params.filterByArea = False

# Filter by Circularity
params.filterByCircularity = True
params.minCircularity = 0.8

# Filter by Convexity
params.filterByConvexity = True
params.minConvexity = 0.8

# Filter by Inertia
params.filterByInertia =True
params.minInertiaRatio = 0.8

# Create SimpleBlobDetector
detector = cv2.SimpleBlobDetector_create(params)

keypoints = detector.detect(imEroded)
print("Number of coins detected = ", len(keypoints))

for point in keypoints:
    x, y = point.pt
    x=int(x)
    y=int(y)
    cv2.circle(image, (x,y), 5, (255, 0, 0), -1)
    radius = int(point.size // 2)
    cv2.circle(image, (x,y), radius, (0, 255, 0), 2)

plt.figure()
plt.imshow(image[:,:,::-1])
plt.title("SimpleBlobDetector detect")
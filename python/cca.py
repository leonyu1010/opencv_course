# -*- coding: utf-8 -*-
"""
Created on Mon Aug  5 15:02:59 2019

@author: SWTestPC
"""

import cv2
import matplotlib.pyplot as plt
import numpy as np
import ImagePath
import matplotlib

matplotlib.rcParams['figure.figsize'] = (10.0, 10.0)
matplotlib.rcParams['image.cmap'] = 'gray'


def displayConnectedComponents(im):
    imLabels = im
    # The following line finds the min and max pixel values
    # and their locations in an image.
    (minVal, maxVal, minLoc, maxLoc) = cv2.minMaxLoc(imLabels)
    # Normalize the image so the min value is 0 and max value is 255.
    imLabels = 255 * (imLabels - minVal)/(maxVal-minVal)
    # Convert image to 8-bits unsigned type
    imLabels = np.uint8(imLabels)
    # Apply a color map
    imColorMap = cv2.applyColorMap(imLabels, cv2.COLORMAP_JET)
    # Display colormapped labels
    plt.figure()
    plt.imshow(imColorMap[:,:,::-1])
    plt.title("displayConnectedComponents")

image_file = "CCA.png"

image = cv2.imread(ImagePath.get_image_path(image_file), 1)
image_clone = image.copy()
plt.figure()
plt.imshow(image[:,:,::-1]);
plt.title("Original Image")

image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
plt.figure()
plt.imshow(image_gray);
plt.title("Gray")

_, image_binary = cv2.threshold(image_gray, 50, 255, cv2.THRESH_BINARY_INV)
plt.figure()
plt.imshow(image_binary);
plt.title("image_binary")

nComponents, imLabels = cv2.connectedComponents(image_binary)
print("number of connected components: ", nComponents)


displayConnectedComponents(imLabels)


contours, hierarchy = cv2.findContours(image_binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
print("Number of contours found = {}".format(len(contours)))
image = image_clone.copy()
cv2.drawContours(image, contours, -1, (255,255,0), 3);
plt.figure()
plt.imshow(image[:,:,::-1])
plt.title("external contours")


params = cv2.SimpleBlobDetector_Params()
params.blobColor = 255
params.minDistBetweenBlobs = 2
# Filter by Area.
params.filterByArea = True
params.minArea = 1;
# Filter by Circularity
params.filterByCircularity = False
params.minCircularity = 0.001
# Filter by Convexity
params.filterByConvexity = False
params.minConvexity = 0.01
# Filter by Inertia
params.filterByInertia = False
params.minInertiaRatio = 0.01
detector = cv2.SimpleBlobDetector_create(params)
plt.figure()
plt.imshow(image_binary);
plt.title("image_binary")
keypoints = detector.detect(image_binary)

print("Number of coins detected = ", len(keypoints))
image = image_clone.copy()
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
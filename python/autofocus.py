# -*- coding: utf-8 -*-
"""
Created on Wed Aug 14 11:02:15 2019

@author: SWTestPC
"""

import cv2
import matplotlib.pyplot as plt
import numpy as np
import ImagePath

import matplotlib
matplotlib.rcParams['figure.figsize'] = (10.0, 10.0)
matplotlib.rcParams['image.interpolation'] = 'bilinear'

def var_abs_laplacian(image):
    kernelSize = 3
    img_blur = cv2.GaussianBlur(image, (kernelSize, kernelSize), 0, 0)
    laplacian = cv2.Laplacian(img_blur, cv2.CV_32F, ksize = kernelSize,
                              scale = 1, delta = 0)

    return np.var(np.absolute(laplacian), dtype=np.float64)

def sum_modified_laplacian(im):
    x_kernel = np.array([-1,2,-1]).reshape(1,3)
    y_kernel = np.array([-1,2,-1]).reshape(3,1)
    x = cv2.filter2D(im, cv2.CV_32F, x_kernel)
    y = cv2.filter2D(im, cv2.CV_32F, y_kernel)
    ml = np.absolute(x) + np.absolute(y)
    return ml.sum()


video_file = ImagePath.get_image_path("focus-test.mp4")

cap = cv2.VideoCapture(video_file)

ret, frame = cap.read()
print("Total frames: {}".format(int(cap.get(cv2.CAP_PROP_FRAME_COUNT))))

# Specify the ROI for flower in the frame
# UPDATE THE VALUES BELOW
top = 15
left = 420
bottom = 645
right = 1050

maxV1 = 0
maxV2 = 0

# Frame with maximum measure of focus
# Obtained using methods 1 and 2
bestFrame1 = 0
bestFrame2 = 0

# Frame ID of frame with maximum measure
# of focus
# Obtained using methods 1 and 2
bestFrameId1 = 0
bestFrameId2 = 0

#print("shape is {}".format(frame.shape))
#shape is (720, 1280, 3)


# Iterate over all the frames present in the video
while(ret):
    # Crop the flower region out of the frame
    flower = frame[top:bottom, left:right]
    gray = cv2.cvtColor(flower, cv2.COLOR_BGR2GRAY)
    # Get measures of focus from both methods
    val1 = var_abs_laplacian(gray)
    val2 = sum_modified_laplacian(gray)

    frameid = int(cap.get(cv2.CAP_PROP_POS_FRAMES))

    # If the current measure of focus is greater
    # than the current maximum
    if val1 > maxV1 :
        # Revise the current maximum
        maxV1 = val1
        # Get frame ID of the new best frame
        bestFrameId1 = frameid
        # Revise the new best frame
        bestFrame1 = frame.copy()

        print("Frame ID of the best frame [Method 1]: {}".format(bestFrameId1))

    # If the current measure of focus is greater
    # than the current maximum
    if val2 > maxV2 :
        # Revise the current maximum
        maxV2 = val2
        # Get frame ID of the new best frame
        bestFrameId2 = frameid
        # Revise the new best frame
        bestFrame2 = frame.copy()

        print("Frame ID of the best frame [Method 2]: {}".format(bestFrameId2))

    # Read a new frame
    ret, frame = cap.read()


print("================================================")
# Print the Frame ID of the best frame
print("Frame ID of the best frame [Method 1]: {}".format(bestFrameId1))
print("Frame ID of the best frame [Method 2]: {}".format(bestFrameId2))

# Release the VideoCapture object
cap.release()

# Stack the best frames obtained using both methods
out = np.hstack((bestFrame1, bestFrame2))
out_flower = np.hstack((bestFrame1[top:bottom, left:right], bestFrame2[top:bottom, left:right]))
# Display the stacked frames
plt.figure()
plt.imshow(out[:,:,::-1]);
plt.axis('off');

plt.figure()
plt.subplot(2,1,1)
plt.title("frame")
plt.imshow(out[:,:,::-1], cmap="gray")
plt.subplot(2,1,2)
plt.title("flower")
plt.imshow(out_flower[:,:,::-1], cmap='gray')

cv2.imshow("flower1", bestFrame1)
cv2.waitKey(0)
cv2.imshow("flower2", bestFrame2)
cv2.waitKey(0)
cv2.imshow("flower1", bestFrame1[top:bottom, left:right])
cv2.waitKey(0)
cv2.imshow("flower2", bestFrame2[top:bottom, left:right])
cv2.waitKey(0)
cv2.destroyAllWindows()

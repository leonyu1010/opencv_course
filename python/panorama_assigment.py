import cv2
import matplotlib.pyplot as plt
import numpy as np
import ImagePath
import matplotlib

matplotlib.rcParams['figure.figsize'] = (10.0, 10.0)
matplotlib.rcParams['image.cmap'] = 'gray'
import os
dirName = "scene"
DATA_PATH = "C:\\Users\\SWTestPC\\source\\repos\\opencv_playground\\"
imagefiles = [DATA_PATH + "images\\{}\\".format(dirName) + f for f in os.listdir(DATA_PATH+"images\\"+dirName) if f.endswith(".jpg")]
imagefiles.sort()
images = []
destination = "{}_result.png".format(dirName)
plt.figure(figsize=[20,15])

for filename in imagefiles:
    img = cv2.imread(filename)
    images.append(img)
    #plt.figure()
    #plt.imshow(img[:,:,::-1]);
    #plt.title("Original Image")

stitcher = cv2.Stitcher.create(mode = 1)
status, pano = stitcher.stitch(images)
plt.figure()
plt.imshow(pano[:,:,::-1]);
plt.title("Original Image")

# -*- coding: utf-8 -*-
"""
Created on Mon Aug  5 14:18:38 2019

@author: SWTestPC
"""

import matplotlib.pyplot as plt

def is_single_channel(image):
    print(image.shape())
    if(image.shape)

def plot_one(image, title, is_bgr=True):
    plt.figure()

    if(is_single_channel(image) or is_bgr == False):
        plt.imshow(image)
    else:
        plt.imshow(image[:,:,::-1]);

    plt.title("Original Image")
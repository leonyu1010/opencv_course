# -*- coding: utf-8 -*-
"""
Created on Mon Aug  5 14:07:29 2019

@author: SWTestPC
"""
from pathlib import Path, PureWindowsPath


def get_image_path(image_name):
    image_folder = PureWindowsPath(
            r"C:\Users\SWTestPC\source\repos\opencv_playground\images")

    return str(image_folder/image_name)
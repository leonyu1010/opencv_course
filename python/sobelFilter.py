import cv2
import numpy as np
import ImagePath
import pprint
import sys
np.set_printoptions(threshold=sys.maxsize)

filename = ImagePath.get_image_path("jersey.jpg")
image_original = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)

# Check for invalid input
if image_original is None:
    print("Could not open or find the image")

image = image_original # cv2.resize(image_original, None, fx=0.25, fy=0.25)
# Apply sobel filter along x direction
sobelx = cv2.Sobel(image, cv2.CV_32F, 1, 0)
# Apply sobel filter along y direction
sobely = cv2.Sobel(image,cv2.CV_32F,0,1)
sobel = cv2.Sobel(image,cv2.CV_32F, 1, 1)

pprint.pprint(sobelx)
# Normalize image for display

cv2.normalize(sobelx,
                dst = sobelx,
                alpha = 0,
                beta = 1,
                norm_type = cv2.NORM_MINMAX,
                dtype = cv2.CV_32F)
pprint.pprint(sobelx)
cv2.normalize(sobely,
                dst = sobely,
                alpha = 0,
                beta = 1,
                norm_type = cv2.NORM_MINMAX,
                dtype = cv2.CV_32F)
cv2.normalize(sobel,
                dst = sobel,
                alpha = 0,
                beta = 1,
                norm_type = cv2.NORM_MINMAX,
                dtype = cv2.CV_32F)



kernelSize = 3

# Applying laplacian
img1 = cv2.GaussianBlur(image_original,(3,3),0,0)
laplacian = cv2.Laplacian(img1, cv2.CV_32F, ksize = kernelSize,
                            scale = 1, delta = 0)

# Normalize results
cv2.normalize(laplacian,
                dst = laplacian,
                alpha = 0,
                beta = 1,
                norm_type = cv2.NORM_MINMAX,
                dtype = cv2.CV_32F)
cv2.imshow("Sobel X Gradients", sobelx)
cv2.imshow("Sobel Y Gradients", sobely)
cv2.imshow("Sobel", sobel)

cv2.imshow("Laplacian", laplacian)
cv2.waitKey(0)

cv2.destroyAllWindows()
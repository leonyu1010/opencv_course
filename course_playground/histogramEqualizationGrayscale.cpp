#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int equalizeHist_gray(){
string filename = DATA_PATH+"images/IMG_20190811_150700.jpg";
Mat img = imread(filename, IMREAD_GRAYSCALE);
// Equalize histogram
Mat imEq;
equalizeHist(img,imEq);

imshow("Original Image",img);
waitKey(0);
imshow("Histogram Equalized", imEq);
waitKey(0);

return 0;
}

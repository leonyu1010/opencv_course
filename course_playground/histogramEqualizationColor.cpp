#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int hist_eq_color(){
string filename = DATA_PATH+"images/IMG_20190811_150723.jpg";
Mat img = imread(filename);


Mat imEq = img.clone();

// Peform histogram equalization on each channel separately
//vector<Mat> imgChannels(3);
//vector<Mat> imEqChannels(3);
//
//split(img,imgChannels);
//split(imEq,imEqChannels);
//
//for (int i=0; i<3; i++){
//    equalizeHist(imgChannels[i],imEqChannels[i]);
//}
//
//merge(imgChannels,img);
//merge(imEqChannels,imEq);

//imshow("Original Image",img);
//waitKey(0);
//imshow("Histogram Equalized", imEq);
//waitKey(0);

//filename = DATA_PATH + "images/IMG_20190811_150700.jpg";
//img = imread(filename);
Mat imhsv;

cvtColor(img, imhsv, COLOR_BGR2HSV);

vector<Mat> imhsvChannels(3);
split(imhsv,imhsvChannels);

// Perform histogram equalization only on the V channel
equalizeHist(imhsvChannels[2],imhsvChannels[2]);
merge(imhsvChannels,imhsv);
// Convert back to BGR format
cvtColor(imhsv,imEq, COLOR_HSV2BGR);

imwrite(DATA_PATH + "IMG_20190811_150723_1.png", imEq);
//imshow("Original Image",img);
//waitKey(0);
//imshow("Histogram Equalized",imEq);
//waitKey(0);
//destroyAllWindows();
return 0;
}

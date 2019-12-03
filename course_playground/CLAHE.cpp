#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int clahe(){
string filename = DATA_PATH+"images/IMG_20190811_150707.jpg";
Mat img = imread(filename);
Mat imhsv;
cvtColor(img, imhsv, COLOR_BGR2HSV);
Mat imhsvClahe = imhsv.clone();
vector<Mat> imhsvChannels(3);
split(imhsv,imhsvChannels);
// Perform histogram equalization only on the V channel
equalizeHist(imhsvChannels[2],imhsvChannels[2]);
Ptr<CLAHE> clahe = createCLAHE(2.0, Size(8,8));

vector<Mat> imhsvClaheChannels(3);
split(imhsvClahe,imhsvClaheChannels);

clahe->apply(imhsvClaheChannels[2],imhsvClaheChannels[2]);

merge(imhsvChannels,imhsv);
merge(imhsvClaheChannels,imhsvClahe);
Mat imEq, imEqCLAHE;
// Convert back to BGR format
cvtColor(imhsv, imEq, COLOR_HSV2BGR);
cvtColor(imhsvClahe, imEqCLAHE, COLOR_HSV2BGR);

imwrite(DATA_PATH + "IMG_20190811_150707_2.png", imEqCLAHE);

return 0;
}

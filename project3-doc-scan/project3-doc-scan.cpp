// project3-doc-scan.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

// On every area selection, changes are updated through binmask
static void getBinMask(const Mat &comMask, Mat &binMask)
{
	if (comMask.empty() || comMask.type() != CV_8UC1)
		CV_Error(Error::StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)");
	if (binMask.empty() || binMask.rows != comMask.rows || binMask.cols != comMask.cols)
		binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}

int main(int argc, char **argv)
{
	int dst_width = 500;
	int dst_height = static_cast<int>(500 * 1.414);
	Point dst_TL{0, 0};
	Point dst_TR{dst_width, 0};
	Point dst_BL{0, dst_height};
	Point dst_BR{dst_width, dst_height};

	vector<Point> dst_ptrs{dst_TR, dst_TL, dst_BL, dst_BR};

	Mat image, mask, bgdModel, fgdModel, binMask, grabcutImage;

	Mat originalImage = imread("./data/images/form1.jpg");
	image = originalImage.clone();

	mask.create(image.size(), CV_8UC1);
	int topmargin = 150;
	int bottommargin = 20;
	int leftmargin = 20;
	int rightmargin = 20;
	Rect bbox(leftmargin, topmargin, image.cols - (leftmargin + rightmargin), image.rows - (topmargin + bottommargin));
	mask.setTo(Scalar(GC_BGD));
	mask(bbox).setTo(Scalar(GC_PR_FGD));

	grabCut(image, mask, bbox, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
	getBinMask(mask, binMask);
	image.copyTo(grabcutImage, binMask);
	mask *= 255;
	binMask *= 255;
	Mat gray;
	cvtColor(grabcutImage, gray, COLOR_BGR2GRAY);

	Mat binary;
	threshold(gray, binary, 10, 255, THRESH_BINARY);

	Mat eroded;
	erode(binary, eroded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

	vector<Point> contours;
	vector<vector<Point>> contours0;
	vector<Vec4i> hierarchy;

	findContours(eroded, contours0, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	double maxArea = -1;
	double area = -1;
	int contourId = -1;
	for (size_t i = 0; i < contours0.size(); i++)
	{
		area = contourArea(contours0[i]);
		if (area > maxArea)
		{
			maxArea = area;
			contourId = i;
		}
		cout << "Contour #" << i + 1 << " area = " << area << " perimeter = " << arcLength(contours0[i], true) << endl;
	}

	approxPolyDP(Mat(contours0[contourId]), contours, 50, true);

	// Mat rotatedBox = originalImage.clone();
	// RotatedRect rotrect;
	// Point2f rect_points[4];
	// Mat boxPoints2f, boxPointsCov;
	//// Rotated rectangle
	// rotrect = minAreaRect(contours);
	// boxPoints(rotrect, boxPoints2f);
	// boxPoints2f.assignTo(boxPointsCov, CV_32S);
	// polylines(rotatedBox, boxPointsCov, true, Scalar(0, 255, 255), 2);
	// imshow("rotatedBox", rotatedBox);
	// waitKey(1);

	// Mat box = originalImage.clone();

	// vector<vector<Point>> finalContours;
	// finalContours.push_back(contours);
	// drawContours(box, finalContours, 0, Scalar(128, 255, 255), 3, LINE_AA, hierarchy, 0);
	// imshow("box", box);
	// waitKey(1);
	// imwrite("./data/images/box.jpg", box);

	Mat h = findHomography(contours, dst_ptrs);

	Mat finalResult;
	warpPerspective(originalImage, finalResult, h, Size(dst_width, dst_height));
	imwrite("./data/images/final.jpg", finalResult);
	return 0;
}
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
	Mat image, mask, bgdModel, fgdModel, binMask, grabcutImage;

	Mat originalImage = imread("./data/images/book1.jpg");
	image = originalImage.clone();

	mask.create(image.size(), CV_8UC1);
	int topmargin = 250;
	int bottommargin = 80;
	int leftmargin = 60;
	int rightmargin = 60;
	Rect rect(leftmargin, topmargin, originalImage.cols - (leftmargin + rightmargin),
			  originalImage.rows - (topmargin + bottommargin));
	mask.setTo(GC_BGD);
	(mask(rect)).setTo(Scalar(GC_PR_FGD));

	grabCut(image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);

	// for (int i = 0; i < 5; i++)
	//{
	//	grabCut(image, mask, rect, bgdModel, fgdModel, 1);
	//}
	getBinMask(mask, binMask);
	image.copyTo(grabcutImage, binMask);

	// imshow("original", originalImage);
	// waitKey(1);
	imshow("grabcut", grabcutImage);
	waitKey(1);

	Mat gray;
	cvtColor(grabcutImage, gray, COLOR_BGR2GRAY);

	Mat binary;
	threshold(gray, binary, 10, 255, THRESH_BINARY);

	Mat eroded;
	erode(binary, eroded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	imshow("eroded", eroded);
	waitKey(1);

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

	Mat rotatedBox = originalImage.clone();
	RotatedRect rotrect;
	Point2f rect_points[4];
	Mat boxPoints2f, boxPointsCov;
	// Rotated rectangle
	rotrect = minAreaRect(contours);
	boxPoints(rotrect, boxPoints2f);
	boxPoints2f.assignTo(boxPointsCov, CV_32S);
	polylines(rotatedBox, boxPointsCov, true, Scalar(0, 255, 255), 2);
	imshow("rotatedBox", rotatedBox);
	waitKey(1);
	Mat box = originalImage.clone();

	vector<vector<Point>> finalContours;
	finalContours.push_back(contours);
	drawContours(box, finalContours, 0, Scalar(128, 255, 255), 3, LINE_AA, hierarchy, 0);
	imshow("box", box);

	waitKey();
	return 0;
}
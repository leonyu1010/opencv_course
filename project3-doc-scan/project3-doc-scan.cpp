// project3-doc-scan.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
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

	Mat originalImage = imread("./data/images/doc.jpg");
	image = originalImage.clone();

	mask.create(image.size(), CV_8UC1);
	Rect rect(10, 10, originalImage.cols - 20, originalImage.rows - 20);
	mask.setTo(GC_BGD);
	(mask(rect)).setTo(Scalar(GC_PR_FGD));

	grabCut(image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);

	for (int i = 0; i < 5; i++)
	{
		grabCut(image, mask, rect, bgdModel, fgdModel, 1);
	}
	getBinMask(mask, binMask);
	image.copyTo(grabcutImage, binMask);

	imshow("original", originalImage);
	waitKey(1);
	imshow("grabcut", grabcutImage);
	waitKey(0);
	return 0;
}
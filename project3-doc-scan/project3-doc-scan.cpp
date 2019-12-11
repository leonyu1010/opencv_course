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

void GrabcutImage(cv::Mat &image, cv::Mat &grabcutImage)
{
	Mat mask, bgdModel, fgdModel;

	mask.create(image.size(), CV_8UC1);

	int topmargin = 150;
	int bottommargin = 20;
	int leftmargin = 20;
	int rightmargin = 20;
	Rect bbox(leftmargin, topmargin, image.cols - (leftmargin + rightmargin), image.rows - (topmargin + bottommargin));
	mask.setTo(Scalar(GC_BGD));
	mask(bbox).setTo(Scalar(GC_PR_FGD));

	grabCut(image, mask, bbox, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);

	image.copyTo(grabcutImage, mask & 1);
}

void ConvertToBinary(cv::Mat &grabcutImage, cv::Mat &binaryImage)
{
	Mat gray;
	cvtColor(grabcutImage, gray, COLOR_BGR2GRAY);

	Mat binary;
	threshold(gray, binary, 10, 255, THRESH_BINARY);

	erode(binary, binaryImage, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
}

void FindCorners(cv::Mat &binaryImage, std::vector<cv::Point> &contours)
{
	vector<vector<Point>> contours0;
	vector<Vec4i> hierarchy;

	findContours(binaryImage, contours0, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

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
}

void TransformCorners(std::vector<cv::Point> &src_corners, std::vector<cv::Point> &dst_corners, cv::Mat &image,
					  cv::Mat &finalResult, int dst_width, int dst_height)
{
	Mat h = findHomography(src_corners, dst_corners);

	warpPerspective(image, finalResult, h, Size(dst_width, dst_height));
	imwrite("./data/images/final.jpg", finalResult);
}

int main(int argc, char **argv)
{
	int dst_width = 500;
	int dst_height = static_cast<int>(500 * 1.414);
	Point dst_TL{0, 0};
	Point dst_TR{dst_width, 0};
	Point dst_BL{0, dst_height};
	Point dst_BR{dst_width, dst_height};

	vector<Point> dst_corners{dst_TR, dst_TL, dst_BL, dst_BR};
	vector<Point> src_corners;

	Mat image = imread("./data/images/form1.jpg");
	Mat grabcutImage, binaryImage, finalResult;

	GrabcutImage(image, grabcutImage);

	ConvertToBinary(grabcutImage, binaryImage);

	FindCorners(binaryImage, src_corners);

	TransformCorners(src_corners, dst_corners, image, finalResult, dst_width, dst_height);
	return 0;
}
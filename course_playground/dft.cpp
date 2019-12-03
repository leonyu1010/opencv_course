#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

void take_dft_gray(Mat& src, Mat& dst)
{
	Mat srcFloat;
	src.convertTo(srcFloat, CV_32FC1, 1.0 / 255.0);

	Mat complexArray[2] = { srcFloat, Mat::zeros(src.size(), CV_32FC1) };

	Mat dftReady;

	merge(complexArray, 2, dftReady);

	dft(dftReady, dst, DFT_COMPLEX_OUTPUT);
}

void recenterDFT(Mat& src)
{
	int cx = src.cols / 2;
	int cy = src.rows / 2;

	Mat q1(src, Rect(0, 0, cx, cy));
	Mat q2(src, Rect(cx, 0, cx, cy));
	Mat q3(src, Rect(0, cy, cx, cy));
	Mat q4(src, Rect(cx, cy, cx, cy));

	Mat tmp;

	q1.copyTo(tmp);
	q4.copyTo(q1);
	tmp.copyTo(q4);

	q2.copyTo(tmp);
	q3.copyTo(q2);
	tmp.copyTo(q3);

	imshow("center dft", src);
	waitKey();
}
void show_dft(Mat& src)
{
	Mat splitArray[2] = { Mat::zeros(src.size(), CV_32FC1), Mat::zeros(src.size(), CV_32FC1) };

	split(src, splitArray);

	Mat mag;

	magnitude(splitArray[0], splitArray[1], mag);

	mag += Scalar::all(1);
	log(mag, mag);
	normalize(mag, mag, 0, 1, NORM_MINMAX);

	imshow("dft", mag);
	waitKey();

	recenterDFT(mag);
}

void inverse_dft(Mat& src, Mat& dst)
{
	dft(src, dst, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
}

int main()
{
	Mat img = imread("C:\\Users\\leony\\source\\repos\\opencv_playground\\images\\trump.jpg", IMREAD_GRAYSCALE);
	Mat dft_img;
	imshow("img", img);
	waitKey();

	take_dft_gray(img, dft_img);

	show_dft(dft_img);

	Mat inversed;
	inverse_dft(dft_img, inversed);
	imshow("inversed", inversed);
	waitKey();
}
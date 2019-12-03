#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "dataPath.hpp"

using namespace std;
using namespace cv;

Mat convertBGRtoGray(Mat image) {
	///
	/// YOUR CODE HERE
	///
	int height = image.rows;
	int width = image.cols;
	Mat gray = Mat(height, width, CV_8UC1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			uchar b, g, r;
			b = image.at<Vec3b>(i, j)[0];
			g = image.at<Vec3b>(i, j)[1];
			r = image.at<Vec3b>(i, j)[2];

			gray.at<uchar>(i, j) = int(0.299 * r + 0.587 * g + 0.114 * b + 0.5);
		}
	}
	return gray;
}

#include <algorithm>
Mat convertBGRtoHSV(Mat image) {
	///
	/// YOUR CODE HERE
	///
	int height = image.rows;
	int width = image.cols;

	Mat hsv = Mat(height, width, CV_8UC3);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float b = float(image.at<Vec3b>(i, j)[0]) / 255;
			float g = float(image.at<Vec3b>(i, j)[1]) / 255;
			float r = float(image.at<Vec3b>(i, j)[2]) / 255;

			float max_val = max({ b, g, r });
			float min_val = min(b, min(g, r));
			float delta = max_val - min_val;

			float v = max_val;
			float s = (v == 0.0) ? 0.0 : delta / v;
			float h = 0.0;
			if (delta != 0.0)
			{
				if (v == r)
				{
					h = 60 * (g - b) / delta;
				}
				else if (v == g)
				{
					h = 120 + 60 * (b - r) / delta;
				}
				else if (v == b)
				{
					h = 240 + 60 * (r - g) / delta;
				}
			}

			if (h < 0.0)
			{
				h = h + 360;
			}

			hsv.at<Vec3b>(i, j)[0] = int(h / 2 + 0.5);
			hsv.at<Vec3b>(i, j)[1] = int(s * 255 + 0.5);
			hsv.at<Vec3b>(i, j)[2] = int(v * 255 + 0.5);
		}
	}
	return hsv;
}
double var_abs_laplacian(Mat image) {
	///
	/// YOUR CODE HERE
	///
	int ksize = 3;
	Mat img_blur;
	GaussianBlur(image, img_blur, Size(ksize, ksize), 0, 0);

	Mat laplacian;
	Laplacian(img_blur, laplacian, CV_32F, ksize);

	Scalar m, stddev;
	meanStdDev(abs(laplacian), m, stddev);
	return stddev[0] * stddev[0];
}

double sum_modified_laplacian(Mat image) {
	///
	/// YOUR CODE HERE
	///
	float vals[3] = { -1.0, 2.0, -1.0 };
	Mat x_filter = Mat(1, 3, CV_32F, vals);
	Mat y_filter = Mat(3, 1, CV_32F, vals);

	Mat x_gradient, y_gradient;
	filter2D(image, x_gradient, CV_32F, x_filter);
	filter2D(image, y_gradient, CV_32F, y_filter);

	Mat ml = abs(x_gradient) + abs(y_gradient);
	return sum(ml)[0];
}

int autofocus()
{
	string file = DATA_PATH + "images\\focus-test.mp4";

	cv::VideoCapture cap(file);

	// Display total number of frames in the video
	cout << "Total number of frames : " << (int)cap.get(CAP_PROP_FRAME_COUNT) << endl;

	double maxV1 = 0;
	double maxV2 = 0;

	// Frame with maximum measure of focus
	// Obtained using methods 1 and 2
	Mat bestFrame1;
	Mat bestFrame2;

	// Frame ID of frame with maximum measure
	// of focus
	// Obtained using methods 1 and 2
	int bestFrameId1 = 0;
	int bestFrameId2 = 0;

	// Get measures of focus from both methods
	double val1 = 0.0;
	double val2 = 0.0;

	// Specify the ROI for flower in the frame
	// UPDATE THE VALUES BELOW
	int topCorner = 15;
	int leftCorner = 420;
	int bottomCorner = 645;
	int rightCorner = 1050;

	while (1)
	{
		cv::Mat frame, flower, gray;

		cap >> frame;
		if (frame.empty())
		{
			break;
		}

		int frameId = (int)cap.get(CAP_PROP_POS_FRAMES);

		flower = frame(Range(topCorner, bottomCorner), Range(leftCorner, rightCorner));
		cv::cvtColor(flower, gray, cv::COLOR_BGR2GRAY);

		// Get measures of focus from both methods
		val1 = var_abs_laplacian(gray);
		val2 = sum_modified_laplacian(gray);
		//cout << "val1: " << std::setprecision(2) << std::fixed << val1 << endl;
		//cout << "val2: " << std::setprecision(2) << std::fixed << val2 << endl;
		// If the current measure of focus is greater
		// than the current maximum
		if (val1 > maxV1) {
			// Revise the current maximum
			maxV1 = val1;
			// Get frame ID of the new best frame
			bestFrameId1 = frameId;
			// Revise the new best frame
			bestFrame1 = frame.clone();
			cout << "Frame ID of the best frame [Method 1]: " << bestFrameId1 << " value: " << std::setprecision(2) << std::fixed << maxV1 << endl;
		}
		// If the current measure of focus is greater
		// than the current maximum
		if (val2 > maxV2) {
			// Revise the current maximum
			maxV2 = val2;
			// Get frame ID of the new best frame
			bestFrameId2 = frameId;
			// Revise the new best frame
			bestFrame2 = frame.clone();
			cout << "Frame ID of the best frame [Method 2]: " << bestFrameId2 << " value: " << std::setprecision(2) << std::fixed << maxV2 << endl;
		}
	}

	cout << "Final method 1: " << bestFrameId1 << " value: " << std::setprecision(2) << std::fixed << maxV1 << endl;
	cout << "Final method 2: " << bestFrameId2 << " value: " << std::setprecision(2) << std::fixed << maxV2 << endl;

	cv::imshow("method1", bestFrame1);
	cv::imshow("method2", bestFrame2);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}
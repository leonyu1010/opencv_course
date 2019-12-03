#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <string>

using namespace std;
using namespace cv;

const string imageFile = "C:\\Users\\SWTestPC\\Desktop\\OpenCV_course\\course_playground\\images\\truth.png";

void HandleTrackbarValue(int value, void* userData)
{
	*(static_cast<int*>(userData)) = value;
}

Mat ProcessResize(int scale, bool up, Mat& img)
{
	Mat resized;
	float factor = 1.0;

	if ((scale == 0) || (!up && scale == 100))
	{
		//when scale is 0, return original image;
		//when scale down 100%, return original image;
		return img;
	}

	factor += scale / 100.0f * (up ? 1 : -1);

	resize(img, resized, Size(), factor, factor, INTER_LINEAR);
	return resized;

}

int trackbar()
{
	string windowName = "Resize Image";
	string trackbarValue = "Scale";
	string trackbarType = "Type: \n 0: Scale Up \n 1: Scale Down";

	int scaleValue = 0;
	int maxScale = 100;
	int scaleType = 0;
	int maxType = 1;

	namedWindow(windowName, WINDOW_AUTOSIZE);

	createTrackbar(trackbarValue, windowName, &scaleValue, maxScale, HandleTrackbarValue, &scaleValue);
	createTrackbar(trackbarType, windowName, &scaleType, maxType, HandleTrackbarValue, &scaleType);

	Mat img = imread(imageFile, 1);
	Mat originalImg = img.clone();
	while (1)
	{
		imshow(windowName, img);
		int key = waitKey(2000) & 0xFF;

		if (key == 27)
		{
			break;
		}
		bool scaleUp = scaleType == 0;

		if (!scaleUp && scaleValue == 100)
		{
			//when 100% scale down is selected, set bar position to 0
			setTrackbarPos(trackbarValue, windowName, 0);
		}
		img = ProcessResize(scaleValue, scaleUp, originalImg);
	}

	destroyAllWindows();
	return 0;
}
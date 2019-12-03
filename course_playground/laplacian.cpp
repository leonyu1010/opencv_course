#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void HandleMouseEvent(int action, int x, int y, int flags, void* userData)
{
	Rect* faceRoi = static_cast<Rect*>(userData);
	if (action == EVENT_LBUTTONDOWN)
	{
		faceRoi->width = 0;
		faceRoi->height = 0;

		faceRoi->x = x;
		faceRoi->y = y;
	}
}

void ProcessBlemish(cv::Rect& faceRoi, cv::Mat& img)
{
}

int blemish()
{
	string filename = "C:\\Users\\leony\\source\\repos\\opencv_playground\\images\\blemish.png";// DATA_PATH + "images/trump.jpg";
	string windowName = "Blemish Removal";
	Rect faceRoi(0, 0, 0, 0);
	namedWindow(windowName);

	setMouseCallback(windowName, HandleMouseEvent, &faceRoi);

	Mat image = imread(filename, IMREAD_COLOR);

	while (1)
	{
		imshow(windowName, image);

		int key = waitKey(10) & 0xFF;
		if (key == 27)
		{
			break;
		}

		ProcessBlemish(faceRoi, image);
	}
	destroyAllWindows();
	return 0;
}
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <string>

using namespace std;
using namespace cv;

const string imageFile = "C:\\Users\\SWTestPC\\Desktop\\OpenCV_course\\course_playground\\images\\sample.jpg";
const string faceResultFile = "C:\\Users\\SWTestPC\\Desktop\\OpenCV_course\\course_playground\\images\\face_result.png";

void HandleMouseEvent1(int action, int x, int y, int flags, void* userData)
{
	Rect* faceRoi = static_cast<Rect*>(userData);
	if (action == EVENT_LBUTTONDOWN)
	{
		faceRoi->width = 0;
		faceRoi->height = 0;

		faceRoi->x = x;
		faceRoi->y = y;
	}
	else if (action == EVENT_LBUTTONUP)
	{
		faceRoi->width = x - faceRoi->x;
		faceRoi->height = y - faceRoi->y;
	}
}

void ProcessFaceROI(cv::Rect& faceRoi, cv::Mat& img)
{
	if (faceRoi.width > 0 && faceRoi.height > 0)
	{
		rectangle(img, faceRoi, Scalar(255, 255, 0), 2);

		imwrite(faceResultFile, img(faceRoi));

		faceRoi.width = 0;
		faceRoi.height = 0;
	}
}

int face_box()
{
	Rect faceRoi(0, 0, 0, 0);

	string windowName = "Sample";

	namedWindow(windowName);

	setMouseCallback(windowName, HandleMouseEvent1, &faceRoi);

	Mat img = imread(imageFile, 1);

	int textX = 20;
	int textY = img.rows - 20;

	putText(img, "Choose top left corner and drag to select the face.",
		Point2i(textX, textY),
		FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 1);

	while (1)
	{
		imshow(windowName, img);

		int key = waitKey(10) & 0xFF;
		if (key == 27)
		{
			break;
		}

		ProcessFaceROI(faceRoi, img);
	}

	destroyAllWindows();
	return 0;
}

//// Points to store the vertices
//Point topleft, bottomright;
//// Source image
//Mat source, dummy;
//
//// function which will be called on mouse input
//void drawSquare(int action, int x, int y, int flags, void* userdata)
//{
//	// Action to be taken when left mouse button is pressed
//	if (action == EVENT_LBUTTONDOWN)
//	{
//		topleft = Point(x, y);
//		// Mark the vertex
//		circle(source, topleft, 1, Scalar(255, 255, 0), 2, LINE_AA);
//	}
//	// Action to be taken when left mouse button is released
//	else if (action == EVENT_LBUTTONUP)
//	{
//		bottomright = Point(x, y);
//		// Draw the square
//		rectangle(source, topleft, bottomright, Scalar(255, 255, 0), 2, LINE_AA);
//		imwrite("face.png", dummy(Range(topleft.y, bottomright.y), Range(topleft.x, bottomright.x)));
//		imshow("Window", source);
//	}
//
//}
//
//int main()
//{
//	source = imread("../data/images/sample.jpg", 1);
//	// Make a dummy image, will be useful to clear the drawing
//	dummy = source.clone();
//	namedWindow("Window");
//	// highgui function called when mouse events occur
//	setMouseCallback("Window", drawSquare);
//	int k = 0;
//	// loop until escape character is pressed
//	while (k != 27)
//	{
//		imshow("Window", source);
//		putText(source, "Choose top left corner, and drag, Press ESC to exit and c to clear", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
//		k = waitKey(20) & 0xFF;
//		if (k == 99)
//			// Another way of cloning
//			dummy.copyTo(source);
//	}
//	return 0;
//}
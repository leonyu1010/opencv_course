// course_playground.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/features2d.hpp>


#include <string>
#include <vector>
using namespace std;
using namespace cv;


const string images = "C:\\Users\\SWTestPC\\source\\repos\\opencv_playground\\images\\";
const string window_name = "Windows";

string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

void qrcode()
{
	string imgPath = "C:\\Users\\SWTestPC\\Desktop\\OpenCV_course\\Computer_Vision_1_Introduction_cpp\\week1-cpp\\data\\images\\IDCard-Satya.png";
	Mat img = imread(imgPath);
	cout << img.size().height << " " << img.size().width;

	Mat bbox, rectifiedImage;

	QRCodeDetector qrDecoder;

	string opencvData = qrDecoder.detectAndDecode(img, bbox, rectifiedImage);

	if (opencvData.length() > 0)
	{
		cout << "QR Code Detected" << endl;
	}
	else
	{
		cout << "QR Code NOT Detected" << endl;
	}
	cout << type2str(bbox.type()) << endl;
	cout << "rows " << bbox.rows << "cols " << bbox.cols << endl;
	cout << "size" << bbox.size() << endl;
	cout << "size height" << bbox.size().height << "size width" << bbox.size().width << endl;
	cout << bbox << endl;
	cout << bbox.at<float>(0, 0) << endl;
	cout << bbox.at<float>(0, 1) << endl;
	cout << bbox.at<float>(1, 0) << endl;
	cout << bbox.at<float>(1, 1) << endl;
	cout << bbox.at<float>(2, 0) << endl;
	cout << bbox.at<float>(2, 1) << endl;
	cout << bbox.at<float>(3, 0) << endl;
	cout << bbox.at<float>(3, 1) << endl;

	cout << 1 % 4 << endl;
	imshow("rectifiedImage", rectifiedImage);
	waitKey(0);

	int n = bbox.rows;
	for (int i = 0; i < n; i++)
	{
		line(img, Point2i(bbox.at<float>(i, 0), bbox.at<float>(i, 1)),
			Point2i(bbox.at<float>((i + 1) % n, 0), bbox.at<float>((i + 1) % n, 1)), Scalar(255, 0, 0), 3);
	}

	imshow("bbox", img);
	waitKey(0);


}


int keyboard(void)
{
	// Open webcam
	VideoCapture cap(0);
	Mat frame;
	int k = 0;
	// Detect if webcam is working properly
	if (!cap.isOpened())
	{
		cout << "Unable to detect webcam " << "\n";
		return 0;
	}
	else
	{
		cap.set(3, 1240);
		cap.set(4, 720);

		while (1)
		{
			// Capture frame
			cap >> frame;
			if (k == 27)
				break;
			// Identify if 'e' or 'E' is pressed
			if (k == 101 || k == 69)
				putText(frame, "E is pressed, press Z or ESC", Point(100, 180), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 3);
			// Identify if 'z' or 'Z' is pressed or not
			if (k == 90 || k == 122)
				putText(frame, "Z is pressed", Point(100, 180), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 3);
			imshow("Image", frame);
			// Waitkey is increased so that the display is shown
			k = waitKey(10000) & 0xFF;
		}
	}
	cap.release();
	destroyAllWindows();

}

// Points to store the center of the circle and a point on the circumference
Point center, circumference;
// Source image
Mat source;

// function which will be called on mouse input
void drawCircle(int action, int x, int y, int flags, void* userdata)
{
	// Action to be taken when left mouse button is pressed
	if (action == EVENT_LBUTTONDOWN)
	{
		center = Point(x, y);
		// Mark the center
		circle(source, center, 1, Scalar(255, 255, 0), 2, LINE_AA);
	}
	// Action to be taken when left mouse button is released
	else if (action == EVENT_LBUTTONUP)
	{
		circumference = Point(x, y);
		// Calculate radius of the circle
		float radius = sqrt(pow(center.x - circumference.x, 2) + pow(center.y - circumference.y, 2));
		// Draw the circle
		circle(source, center, radius, Scalar(0, 255, 0), 2, LINE_AA);
		imshow("Window", source);
	}

}

int mouseEvent()
{
	source = imread(images + "sample.jpg", 1);
	// Make a dummy image, will be useful to clear the drawing
	Mat dummy = source.clone();
	namedWindow("Window");
	// highgui function called when mouse events occur
	setMouseCallback("Window", drawCircle);
	int k = 0;
	// loop until escape character is pressed
	while (k != 27)
	{
		imshow("Window", source);
		putText(source, "Choose center, and drag, Press ESC to exit and c to clear", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
		k = waitKey(20) & 0xFF;
		if (k == 99)
			// Another way of cloning
			dummy.copyTo(source);
	}
	return 0;
}

int run_threshold()
{
	Mat img = imread(images + "dilation_example.jpg", IMREAD_COLOR);
	Mat new_img;

	//threshold(img, new_img, 100, 255, THRESH_TOZERO_INV);

	imshow(window_name, img);
	
	waitKey(0);

	return 0;
}

int dilate_1()
{
	Mat demoImage = Mat::zeros(Size(10, 10), CV_8U);
	demoImage.at<uchar>(0, 1) = 1;
	demoImage.at<uchar>(9, 0) = 1;
	demoImage.at<uchar>(8, 9) = 1;
	demoImage.at<uchar>(2, 2) = 1;
	demoImage(Range(5, 8), Range(5, 8)).setTo(1);

	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	int ksize = element.size().height;
	int height, width;
	height = demoImage.size().height;
	width = demoImage.size().width;

	int border = ksize / 2;
	Mat paddedDemoImage = Mat::zeros(Size(height + border * 2, width + border * 2), CV_8UC1);
	copyMakeBorder(demoImage, paddedDemoImage, border, border, border, border, BORDER_CONSTANT, 0);
	Mat paddedDilatedImage = paddedDemoImage.clone();
	Mat paddedErodedImage = paddedDemoImage.clone();
	Mat mask;

	Mat resizedFrame;
	Mat bgrFrame;
	cout << paddedDemoImage << endl;
	double minVal, maxVal;
	int sumOfElement = sum(element)[0];
	// Create a VideoWriter object
	///
	/// YOUR CODE HERE
	///
	VideoWriter outavi(images+"dilationScratch.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(50, 50));

	for (int h_i = border; h_i < height + border; h_i++) {
		for (int w_i = border; w_i < width + border; w_i++) {
			///
			/// YOUR CODE HERE
			///
			Mat and_result;

			//bitwise_and(paddedDemoImage(Range(h_i - border, h_i + border + 1), Range(w_i - border, w_i + border + 1)),
			//	element, mask);

			//minMaxLoc(mask, &minVal, &maxVal);
			//
			//paddedDilatedImage.at<uchar>(h_i, w_i) = static_cast<uchar>(maxVal);


			bitwise_and(paddedDemoImage(Range(h_i - border, h_i + border + 1), Range(w_i - border, w_i + border + 1)),
				element, and_result);

			int s = sum(and_result)[0];
			if (s == sumOfElement)
			{
				paddedErodedImage.at<uchar>(h_i, w_i) = 1;
			}
			else
			{
				paddedErodedImage.at<uchar>(h_i, w_i) = 0;
			}

			resize(paddedErodedImage, resizedFrame, Size(50, 50));
			cvtColor(resizedFrame*255, bgrFrame, COLOR_GRAY2BGR);
			outavi.write(bgrFrame);

			// Resize output to 50x50 before writing it to the video
			///
			/// YOUR CODE HERE
			///
			// Convert resizedFrame to BGR before writing
			///
			/// YOUR CODE HERE
			///
		}
	}

	cout << paddedErodedImage << endl;
	// Release the VideoWriter object
	///
	/// YOUR CODE HERE
	///
	outavi.release();

	return 0;
}

Mat displayConnectedComponents(Mat& im)
{
	Mat imLabels = im.clone();

	Point minLoc, maxLoc;
	double min, max;

	minMaxLoc(imLabels, &min, &max, &minLoc, &maxLoc);

	imLabels = 255 * (imLabels - min) / (max - min);

	imLabels.convertTo(imLabels, CV_8U);

	Mat imColorMap;
	applyColorMap(imLabels, imColorMap, COLORMAP_JET);

	return imColorMap;
}

int coin_2()
{
	Mat image = imread(images + "CoinsB.png", IMREAD_COLOR);
	Mat imageCopy = image.clone();

	Mat imageGray;
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	Mat channels[3];
	split(image, channels);
	Mat imageB = channels[0];
	Mat imageG = channels[1];
	Mat imageR = channels[2];

	Mat src = imageGray;
	int thresh = 150;
	int maxValue = 255;

	Mat imThresh;
	threshold(src, imThresh, thresh, maxValue, THRESH_BINARY);
	imwrite(images + "imThresh.png", imThresh);

	int kSize0 = 3;
	Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize0, kSize0));
	Mat imageMorphClosed;
	morphologyEx(imThresh, imageMorphClosed, MORPH_CLOSE, element, Point(-1,-1), 5);
	imwrite(images + "imageMorphClosed.png", imageMorphClosed);

	kSize0 = 21;
	element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize0, kSize0));
	Mat imageMorphOpen;
	morphologyEx(imageMorphClosed, imageMorphOpen, MORPH_OPEN, element, Point(-1, -1), 6);
	imwrite(images + "imageMorphOpen.png", imageMorphOpen);




	//int kSize1 = 7;
	//Mat kernel1= getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(kSize1, kSize1));
	//Mat imageDilated;
	//dilate(imageMorphOpen, imageDilated, kernel1, Point(-1,-1), 5);
	//imwrite(images + "imageDilated.png", imageDilated);

	//int kSize2 = 15;
	//Mat kernel2 = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize2, kSize2));
	//Mat imageEroded;
	//erode(imageMorphOpen, imageEroded, kernel2, Point(-1, -1), 5);
	//imwrite(images + "imageEroded.png", imageEroded);

	Mat imageSimpleBlobDetector = imageMorphOpen.clone();

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;
	params.blobColor = 0;

	params.minDistBetweenBlobs = 2;

	// Filter by Area
	params.filterByArea = false;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.8;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.8;
	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	vector<KeyPoint> keypoints;
	detector->detect(imageSimpleBlobDetector, keypoints);
	cout << "number of coins detected: " << keypoints.size() << endl;
	int x, y;
	int radius;
	double diameter;
	for (int i = 0; i < keypoints.size(); i++) {
		KeyPoint k = keypoints[i];
		Point keyPt;
		keyPt = k.pt;
		x = (int)keyPt.x;
		y = (int)keyPt.y;
		// Mark center in BLACK
		circle(image, Point(x, y), 5, Scalar(255, 0, 0), -1);
		// Get radius of coin
		diameter = k.size;
		radius = (int)diameter / 2.0;
		// Mark blob in GREEN
		circle(image, Point(x, y), radius, Scalar(0, 0, 255), 4);
	}
	imwrite(images+"SimpleBlobDetector.png", image);

	Mat imageBinary;
	bitwise_not(imageMorphOpen, imageBinary);
	imwrite(images + "binary.png", imageBinary);

	kSize0 = 21;
	element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize0, kSize0));
	Mat binaryOpen;
	morphologyEx(imageBinary, binaryOpen, MORPH_OPEN, element, Point(-1, -1), 3);
	imwrite(images + "binaryOpen.png", binaryOpen);

	imageBinary = binaryOpen.clone();
	Mat imLabels;
	int nComponents = connectedComponents(imageBinary, imLabels);
	cout << "number of connected components: " << nComponents << endl;
	Mat colorMap = displayConnectedComponents(imLabels);
	imwrite(images + "ColorMap.png", colorMap);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imageBinary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << "Number of contours found = " << contours.size() << endl;

	Mat imageContours = imageCopy.clone();
	drawContours(imageContours, contours, -1, Scalar(255, 0, 0), 10);
	Moments M;
	for (size_t i = 0; i < contours.size(); i++) {
		// We will use the contour moments
		// to find the centroid
		M = moments(contours[i]);
		x = int(M.m10 / double(M.m00));
		y = int(M.m01 / double(M.m00));

		// Mark the center
		circle(imageContours, Point(x, y), 5, Scalar(255, 0, 0), 10);

		// Mark the contour number
		putText(imageContours, to_string(i + 1), Point(x + 40, y - 10), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 5);
	}
	imwrite(images+"imageContours.png", imageContours);
	double area;
	double perimeter;
	for (size_t i = 0; i < contours.size(); i++) {
		area = contourArea(contours[i]);
		perimeter = arcLength(contours[i], true);
		cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << endl;
	}

	Mat imageFitCircle = imageCopy.clone();
	Point2f center;
	float fitRadius;
	for (size_t i = 0; i < contours.size(); i++) {
		// Fit a circle
		minEnclosingCircle(contours[i], center, fitRadius);
		circle(imageFitCircle, center, fitRadius, Scalar(255, 0, 0), 5);
		circle(imageFitCircle, center, 5, Scalar(0, 0, 255), 10);
	}
	imwrite(images+"imageFitCircle.png", imageFitCircle);

	return 0;
}
int coin()
{
	Mat image = imread(images + "CoinsA.png", IMREAD_COLOR);
	Mat imageCopy = image.clone();

	Mat imageGray;
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	Mat channels[3];
	split(image, channels);
	Mat imageB = channels[0];
	Mat imageG = channels[1];
	Mat imageR = channels[2];
	imshow("original", image);
	imshow("Gray", imageGray);
	imshow("B", imageB);
	imshow("G", imageG);
	imshow("R", imageR);

	Mat src = imageG;
	int thresh = 30;
	int maxValue = 255;

	//Mat dst_bin;
	//threshold(src, dst_bin, thresh, maxValue, THRESH_BINARY);
	////adaptiveThreshold(src, dst_bin, maxValue, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 0);
	//imshow("THRESH_BINARY", dst_bin);


	Mat dst_bin_inv;
	threshold(src, dst_bin_inv, thresh, maxValue, THRESH_BINARY_INV);

	imshow("THRESH_BINARY_INV", dst_bin_inv);

	int kSize1 = 7;
	Mat kernel1 = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize1, kSize1));

	int kSize2 = 9;
	Mat kernel2 = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize2, kSize2));

	Mat imageDilated;
	Mat imageEroded;

	dilate(dst_bin_inv, imageDilated, kernel1, Point(-1,-1), 1);
	imshow("imageDilated", imageDilated);

	erode(imageDilated, imageEroded, kernel2, Point(-1, -1), 1);
	imshow("imageEroded", imageEroded);

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	params.blobColor = 0;

	params.minDistBetweenBlobs = 2;

	// Filter by Area
	params.filterByArea = false;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.8;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.8;

	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	vector<KeyPoint> keypoints;
	detector->detect(imageEroded, keypoints);
	cout << "number of coins detected: " << keypoints.size() << endl;

	int x, y;
	int radius;
	double diameter;
	for (int i = 0; i < keypoints.size(); i++) {
		KeyPoint k = keypoints[i];
		Point keyPt;
		keyPt = k.pt;
		x = (int)keyPt.x;
		y = (int)keyPt.y;
		// Mark center in BLACK
		circle(image, Point(x, y), 5, Scalar(255, 0, 0), -1);
		// Get radius of coin
		diameter = k.size;
		radius = (int)diameter / 2.0;
		// Mark blob in GREEN
		circle(image, Point(x, y), radius, Scalar(0, 255, 0), 2);
	}
	imshow("SimpleBlobDetector", image);

	Mat imThresh;
	threshold(imageG, imThresh, 30, 255, THRESH_BINARY);
	imshow("imThresh", imThresh);
	
	int kSize3 = 7;
	Mat kernel3 = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize3, kSize3));

	Mat imageMorphOpen;
	morphologyEx(imThresh, imageMorphOpen, MORPH_OPEN, kernel3, Point(-1, -1), 1);
	imshow("imageMorphOpen", imageMorphOpen * 255);

	Mat imLabels;
	int nComponents = connectedComponents(imageMorphOpen, imLabels);
	cout << "number of connected components: " << nComponents << endl;
	Mat colorMap = displayConnectedComponents(imLabels);
	imshow("ColorMap", colorMap);



	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imageMorphOpen, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << "Number of contours found = " << contours.size() << endl;

	Mat imageContours = imageCopy.clone();
	drawContours(imageContours, contours, -1, Scalar(0, 255, 0), 3);

	imshow("imageContours", imageContours);
	double area;
	double perimeter;
	for (size_t i = 0; i < contours.size(); i++) {
		area = contourArea(contours[i]);
		perimeter = arcLength(contours[i], true);
		cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << endl;
	}
	int small = 0;
	contours.erase(contours.begin() + small);
	Mat imageFitCircle = imageCopy.clone();
	Point2f center;
	float fitRadius;
	for (size_t i = 0; i < contours.size(); i++) {
		// Fit a circle
		minEnclosingCircle(contours[i], center, fitRadius);
		circle(imageFitCircle, center, fitRadius, Scalar(0, 255, 0), 2);
		circle(imageFitCircle, center, 1, Scalar(0, 0, 255), 5);
	}
	imshow("imageFitCircle", imageFitCircle);

	//Mat dst_trunc;
	//threshold(src, dst_trunc, thresh, maxValue, THRESH_TRUNC);

	//imshow("THRESH_TRUNC", dst_trunc);


	//Mat dst_to_zero;
	//threshold(src, dst_to_zero, thresh, maxValue, THRESH_TOZERO);

	//imshow("THRESH_TOZERO", dst_to_zero);


	//Mat dst_to_zero_inv;
	//threshold(src, dst_to_zero_inv, thresh, maxValue, THRESH_TOZERO_INV);

	//imshow("THRESH_TOZERO_INV", dst_to_zero_inv);


	waitKey(0);
	return 0;
}

int car()
{
	Mat image = imread(images + "car_detect.png", 1);
	Mat channels[3];
	split(image, channels);
	Mat imageB = channels[0];
	Mat imageGray;
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	imshow("Gray", imageGray);

	imshow("imageB", imageB);
	Mat imThreshold;
	threshold(imageB, imThreshold, 127, 255, THRESH_BINARY_INV);
	imshow("imThreshold", imThreshold);

	int ksize = 31;
	Mat element = getStructuringElement(MORPH_RECT, Size(ksize, ksize));

	Mat close;
	morphologyEx(imThreshold, close, MORPH_CLOSE, element);
	imshow("close", close);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(close, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << "Number of contours found = " << contours.size() << endl;

	Mat imageContours = image.clone();
	drawContours(imageContours, contours, -1, Scalar(0, 255, 0), 3);
	imshow("contours", imageContours);
	Mat imageDilated;
	dilate(imThreshold, imageDilated, element);
	imshow("dilated", imageDilated);


	Mat imageEroded;
	erode(imageDilated, imageEroded, element);
	imshow("eroded", imageEroded);


	waitKey(0);
	return 0;
}
int test()
{
	//car();
	//coin();
	coin_2();
	//dilate_1();

	destroyAllWindows();
	return 0;
}

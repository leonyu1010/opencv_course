/*
  Copyright 2018 BIG VISION LLC ALL RIGHTS RESERVED
*/

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "getRandomColors.h"
using namespace cv;
using namespace std;

vector<string> trackerTypes = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};

// create tracker by name
Ptr<Tracker> createTrackerByName(string trackerType)
{
	Ptr<Tracker> tracker;
	if (trackerType == trackerTypes[0])
		tracker = TrackerBoosting::create();
	else if (trackerType == trackerTypes[1])
		tracker = TrackerMIL::create();
	else if (trackerType == trackerTypes[2])
		tracker = TrackerKCF::create();
	else if (trackerType == trackerTypes[3])
		tracker = TrackerTLD::create();
	else if (trackerType == trackerTypes[4])
		tracker = TrackerMedianFlow::create();
	else if (trackerType == trackerTypes[5])
		tracker = TrackerGOTURN::create();
	else if (trackerType == trackerTypes[6])
		tracker = TrackerMOSSE::create();
	else if (trackerType == trackerTypes[7])
		tracker = TrackerCSRT::create();
	else
	{
		cout << "Incorrect tracker name" << endl;
		cout << "Available trackers are: " << endl;
		for (vector<string>::iterator it = trackerTypes.begin(); it != trackerTypes.end(); ++it)
			std::cout << " " << *it << endl;
	}
	return tracker;
}

int trackMultiObjects(int argc, char *argv[])
{
	cout << "Default tracking algoritm is CSRT" << endl;
	cout << "Available tracking algorithms are:" << endl;
	for (vector<string>::iterator it = trackerTypes.begin(); it != trackerTypes.end(); ++it)
		std::cout << " " << *it << endl;

	// Set tracker type. Change this to try different trackers.
	string trackerType = "CSRT";

	// set default values for tracking algorithm and video
	string videoPath = "../data/videos/hockey.mp4";

	// Initialize MultiTracker with tracking algo
	vector<Rect> bboxes;

	// create a video capture object to read videos
	cv::VideoCapture cap(videoPath);
	Mat frame;

	// quit if unabke to read video file
	if (!cap.isOpened())
	{
		cout << "Error opening video file " << videoPath << endl;
		return -1;
	}

	// read first frame
	cap >> frame;

	// draw bounding boxes over objects
	// selectROI's default behaviour is to draw box starting from the center
	// when fromCenter is set to false, you can draw box starting from top left corner
	bool showCrosshair = true;
	bool fromCenter = false;
	cout << "\n==========================================================\n";
	cout << "OpenCV says press c to cancel objects selection process" << endl;
	cout << "It doesn't work. Press Escape to exit selection process" << endl;
	cout << "\n==========================================================\n";
	cv::selectROIs("MultiTracker", frame, bboxes, showCrosshair, fromCenter);

	// quit if there are no objects to track
	if (bboxes.size() < 1)
		return 0;

	vector<Scalar> colors;
	getRandomColors(colors, bboxes.size());

	// Create multitracker
	Ptr<MultiTracker> multiTracker = cv::MultiTracker::create();

	// initialize multitracker
	for (int i = 0; i < bboxes.size(); i++)
		multiTracker->add(createTrackerByName(trackerType), frame, Rect2d(bboxes[i]));

	// process video and track objects
	cout << "\n==========================================================\n";
	cout << "Started tracking, press ESC to quit." << endl;
	while (cap.isOpened())
	{
		// get frame from the video
		cap >> frame;

		// stop the program if reached end of video
		if (frame.empty())
			break;

		// update the tracking result with new frame
		multiTracker->update(frame);

		// draw tracked objects
		for (unsigned i = 0; i < multiTracker->getObjects().size(); i++)
		{
			rectangle(frame, multiTracker->getObjects()[i], colors[i], 2, 1);
		}

		// show frame
		imshow("MultiTracker", frame);

		// quit on x button
		if (waitKey(1) == 27)
			break;
	}
}
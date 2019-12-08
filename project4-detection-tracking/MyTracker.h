#pragma once
#include "opencv2/opencv.hpp"
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <string>
using namespace std;
using namespace cv;
typedef struct MyTracker
{
	string trackerType;
	bool IsTracking;
	Ptr<Tracker> tracker;

	void Track(Mat &frame, Rect2d &bbox)
	{
		if (!IsTracking)
		{
			CreateTracker();
			tracker->init(frame, bbox);
		}
		Update(frame, bbox);
	}

	void CreateTracker()
	{
		string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "CSRT", "MOSSE"};

		if (trackerType == "BOOSTING")
			tracker = TrackerBoosting::create();
		else if (trackerType == "MIL")
			tracker = TrackerMIL::create();
		else if (trackerType == "KCF")
			tracker = TrackerKCF::create();
		else if (trackerType == "TLD")
			tracker = TrackerTLD::create();
		else if (trackerType == "MEDIANFLOW")
			tracker = TrackerMedianFlow::create();
		else if (trackerType == "GOTURN")
			tracker = TrackerGOTURN::create();
		else if (trackerType == "CSRT")
			tracker = TrackerCSRT::create();
		else if (trackerType == "MOSSE")
			tracker = TrackerMOSSE::create();
		else
		{
			cout << "INVALID TRACKER SPECIFIED" << endl;
			cout << "Available Trackers are :" << endl;
			for (int i = 0; i < sizeof(trackerTypes) / sizeof(trackerTypes[0]); i++)
				cout << i << " : " << trackerTypes[i] << endl;
			throw;
		}
	}

	void Update(Mat &frame, Rect2d &bbox)
	{
		// Start timer
		double timer = (double)getTickCount();

		// Update the tracking result
		IsTracking = tracker->update(frame, bbox);

		// Calculate Frames per second (FPS)
		float fps = getTickFrequency() / ((double)getTickCount() - timer);

		if (IsTracking)
		{
			// Tracking success : Draw the tracked object
			rectangle(frame, bbox, Scalar(0, 255, 0), 2, 1);
		}
		else
		{
			// Tracking failure detected.
			putText(frame, "Tracking failure detected", Point(100, 180), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255),
					2);
		}

		// Display tracker type on frame
		putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

		// Display FPS on frame
		putText(frame, "FPS : " + std::to_string(fps), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50),
				2);
	}
} MyTracker_t;
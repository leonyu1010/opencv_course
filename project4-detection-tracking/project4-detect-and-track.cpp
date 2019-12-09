// Usage example:  ./object_detection_yolo.out --video=run.mp4
//                 ./object_detection_yolo.out --image=bird.jpg
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "MyTracker.h"
#include "MyDector.h"
#include "MyParser.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	string inputFile, outputFile;
	VideoCapture cap;
	VideoWriter video;
	Mat frame;
	int frameCount = 0;

	MyParser_t parser{"", "", -1};
	parser.Parse(argc, argv);
	inputFile = parser.InputFile;
	outputFile = parser.OutputFile;
	ifstream ifile(inputFile);
	if (!ifile)
	{
		int device = std::atoi(inputFile.c_str());
		cap.open(device);
	}
	else
	{
		cap.open(inputFile);
	}
	video.open(outputFile, VideoWriter::fourcc('M', 'J', 'P', 'G'), 28,
			   Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)));

	MyTracker_t myTracker{"KCF", false};

	string className = "sports ball";
	MyDetector_t myDetector{className};
	myDetector.Init();

	string kWinName = "Detect and Track";
	cv::namedWindow(kWinName, WINDOW_NORMAL);

	while (++frameCount)
	{
		cap >> frame;

		if (frame.empty())
		{
			cout << "Done processing !!!" << endl;
			cout << "Output file is stored as " << outputFile << endl;
			waitKey(3000);
			break;
		}

		if (!myTracker.IsTracking)
		{
			myDetector.Detect(frame);

			if (!myDetector.found)
			{
				video.write(frame);
				cout << className << " not detected" << endl;
				imshow(kWinName, frame);
				waitKey(1);
				continue;
			}
		}

		myTracker.Track(frame, myDetector.bbox);
		video.write(frame);

		imshow(kWinName, frame);
		waitKey(1);
	}

	cap.release();
	if (parser.IsVideoOutput)
	{
		video.release();
	}

	return 0;
}
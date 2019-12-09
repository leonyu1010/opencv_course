#include <iostream>
#include <string>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "MyTracker.h"
#include "MyDetector.h"
#include "MyParser.h"

using namespace cv;
using namespace std;

void InitInputAndOutput(MyParser_t &parser, cv::VideoCapture &cap, cv::VideoWriter &video)
{
	ifstream ifile(parser.InputFile);
	if (!ifile)
	{
		int device = std::atoi(parser.InputFile.c_str());
		cap.open(device);
	}
	else
	{
		cap.open(parser.InputFile);
	}
	video.open(parser.OutputFile, VideoWriter::fourcc('M', 'J', 'P', 'G'), cap.get(CAP_PROP_FPS),
			   Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)));
}

int main(int argc, char **argv)
{
	string className = "sports ball";
	VideoCapture cap;
	VideoWriter video;
	Mat frame;
	int frameCount = 0;

	MyParser_t parser{"./data/videos/soccer-ball.mp4", "./data/videos/soccer-ball_output.avi", true};
	// parser.Parse(argc, argv);

	InitInputAndOutput(parser, cap, video);

	MyTracker_t myTracker{"KCF", false};

	MyDetector_t myDetector{className};
	myDetector.Init();

	// string kWinName = "Detect and Track";
	// cv::namedWindow(kWinName, WINDOW_NORMAL);

	while (++frameCount)
	{
		cap >> frame;

		if (frame.empty())
		{
			cout << "Done processing !!!" << endl;
			cout << "Output file is stored as " << parser.OutputFile << endl;
			waitKey(3000);
			break;
		}

		if (!myTracker.IsTracking)
		{
			myDetector.Detect(frame);

			if (!myDetector.found)
			{
				video.write(frame);
				cout << className << " not detected Frame " << frameCount - 1 << endl;
				// imshow(kWinName, frame);
				// waitKey(1);
				continue;
			}
		}

		myTracker.Track(frame, myDetector.bbox);
		video.write(frame);

		// imshow(kWinName, frame);
		// waitKey(1);
	}

	cap.release();
	if (parser.IsVideoOutput)
	{
		video.release();
	}

	return 0;
}
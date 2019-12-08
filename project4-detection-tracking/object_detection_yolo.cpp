// Usage example:  ./object_detection_yolo.out --video=run.mp4
//                 ./object_detection_yolo.out --image=bird.jpg
#include <fstream>
#include <sstream>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "MyTracker.h"
#include "MyDector.h"
const char *keys = "{help h usage ? | | Usage examples: \n\t\t./object_detection_yolo.out --image=dog.jpg "
				   "\n\t\t./object_detection_yolo.out --video=../data/videos/run_sm.mp4}"
				   "{image i        |<none>| input image   }"
				   "{video v       |<none>| input video   }";

using namespace cv;
using namespace std;

// Remove the bounding boxes with low confidence using non-maxima suppression
void postprocess(Mat &frame, const vector<Mat> &out);

// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame);

// Get the names of the output layers
vector<String> getOutputsNames(const Net &net);

int main(int argc, char **argv)
{
	CommandLineParser parser(argc, argv, keys);
	parser.about("Use this script to run object detection using YOLO3 in OpenCV.");
	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}

	// Open a video file or an image file or a camera stream.
	string str, outputFile;
	VideoCapture cap;
	VideoWriter video;
	Mat frame;
	int frameCount = 0;

	MyTracker_t myTracker{"KCF", false};

	MyDetector_t myDetector{"sports ball"};
	myDetector.Init();

	try
	{
		outputFile = "yolo_out_cpp.avi";
		if (parser.has("image"))
		{
			// Open the image file
			str = parser.get<String>("image");
			ifstream ifile(str);
			if (!ifile)
				throw("error");
			cap.open(str);
			str.replace(str.end() - 4, str.end(), "_yolo_out_cpp.jpg");
			outputFile = str;
		}
		else if (parser.has("video"))
		{
			// Open the video file
			str = parser.get<String>("video");
			ifstream ifile(str);
			if (!ifile)
				throw("error");
			cap.open(str);
			str.replace(str.end() - 4, str.end(), "_yolo_out_cpp.avi");
			outputFile = str;
		}
		// Open the webcaom
		else
			cap.open(parser.get<int>("device"));
	}
	catch (...)
	{
		cout << "Could not open the input image/video stream" << endl;
		return 0;
	}

	// Get the video writer initialized to save the output video
	if (!parser.has("image"))
	{
		video.open(outputFile, VideoWriter::fourcc('M', 'J', 'P', 'G'), 28,
				   Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)));
	}

	// Create a window
	// static const string kWinName = "Deep learning object detection in OpenCV";
	// cv::namedWindow(kWinName, WINDOW_NORMAL);

	// Process frames.
	while (++frameCount)
	{
		// get frame from the video
		cap >> frame;

		// Stop the program if reached end of video
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
				continue;
			}
		}

		myTracker.Track(frame, myDetector.bbox);
		video.write(frame);

		// imshow(kWinName, frame);
		// waitKey(1);
	}

	cap.release();
	if (!parser.has("image"))
		video.release();

	return 0;
}
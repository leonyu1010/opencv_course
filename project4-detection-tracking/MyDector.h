#pragma once
#include "opencv2/opencv.hpp"
#include <opencv2/dnn.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace cv;
using namespace cv::dnn;

// Initialize the parameters
float confThreshold = 0.5; // Confidence threshold
float nmsThreshold = 0.4;  // Non-maximum suppression threshold
int inpWidth = 416;		   // Width of network's input image
int inpHeight = 416;	   // Height of network's input image

typedef struct MyDetector
{
	string searchClass;
	Net net;
	vector<string> classes;
	vector<string> names;
	bool found;
	Rect2d bbox;
	void Init()
	{
		// Load names of classes
		string classesFile = "./data/models/coco.names";
		ifstream ifs(classesFile.c_str());
		string line;
		while (getline(ifs, line))
			classes.push_back(line);

		// Give the configuration and weight files for the model
		String modelConfiguration = "./data/models/yolov3.cfg";
		String modelWeights = "./data/models/yolov3.weights";

		// Load the network
		net = readNetFromDarknet(modelConfiguration, modelWeights);
		getOutputsNames();
	}

	bool Detect(Mat &frame)
	{
		Mat blob;
		// Create a 4D blob from a frame.
		blobFromImage(frame, blob, 1 / 255.0, Size(inpWidth, inpHeight), Scalar(0, 0, 0), true, false);

		// Sets the input to the network
		net.setInput(blob);

		// Runs the forward pass to get output of the output layers
		vector<Mat> outs;
		net.forward(outs, names);

		// Remove the bounding boxes with low confidence
		postprocess(frame, outs);
		return found;
	}

	// Remove the bounding boxes with low confidence using non-maxima suppression
	void postprocess(Mat &frame, const vector<Mat> &outs)
	{
		vector<int> classIds;
		vector<float> confidences;
		vector<Rect> boxes;
		found = false;

		for (size_t i = 0; i < outs.size(); ++i)
		{
			// Scan through all the bounding boxes output from the network and keep only the
			// ones with high confidence scores. Assign the box's class label as the class
			// with the highest score for the box.
			float *data = (float *)outs[i].data;
			for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
			{
				Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
				Point classIdPoint;
				double confidence;
				// Get the value and location of the maximum score
				minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
				if (searchClass != classes[classIdPoint.x])
				{
					continue;
				}
				if (confidence > confThreshold)
				{
					int centerX = (int)(data[0] * frame.cols);
					int centerY = (int)(data[1] * frame.rows);
					int width = (int)(data[2] * frame.cols);
					int height = (int)(data[3] * frame.rows);
					int left = centerX - width / 2;
					int top = centerY - height / 2;

					classIds.push_back(classIdPoint.x);
					confidences.push_back((float)confidence);
					boxes.push_back(Rect(left, top, width, height));
				}
			}
		}

		// Perform non maximum suppression to eliminate redundant overlapping boxes with
		// lower confidences
		vector<int> indices;
		NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
		for (size_t i = 0; i < indices.size(); ++i)
		{
			found = true;
			int idx = indices[i];
			Rect box = boxes[idx];
			drawPred(classIds[idx], confidences[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame);

			bbox = box;
		}

		if (!found)
		{
			putText(frame, "Object not detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
		}
		else
		{
			// Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the
			// timings for each of the layers(in layersTimes)
			vector<double> layersTimes;
			double freq = getTickFrequency() / 1000;
			double t = net.getPerfProfile(layersTimes) / freq;
			string label = format("Detector FPS : %.2f", 1000.0f / t);
			putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
		}
	}

	// Draw the predicted bounding box
	void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame)
	{
		// Draw a rectangle displaying the bounding box
		rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 0, 0), 3);

		// Get the label for the class name and its confidence
		string label = format("%.2f", conf);
		if (!classes.empty())
		{
			CV_Assert(classId < (int)classes.size());
			label = classes[classId] + ":" + label;
		}

		// Display the label at the top of the bounding box
		int baseLine;
		Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
		top = max(top, labelSize.height);
		rectangle(frame, Point(left, top - round(2 * labelSize.height)),
				  Point(left + round(1.5 * labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
		putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);
	}

	// Get the names of the output layers
	void getOutputsNames()
	{
		if (names.empty())
		{
			// Get the indices of the output layers, i.e. the layers with unconnected outputs
			vector<int> outLayers = net.getUnconnectedOutLayers();

			// get the names of all the layers in the network
			vector<String> layersNames = net.getLayerNames();

			// Get the names of the output layers in names
			names.resize(outLayers.size());
			for (size_t i = 0; i < outLayers.size(); ++i)
				names[i] = layersNames[outLayers[i] - 1];
		}
	}
} MyDetector_t;
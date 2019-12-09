#pragma once
const char *keys = "{help h usage ? | | Usage examples: \n\t\t./object_detection_yolo.out --image=dog.jpg "
				   "\n\t\t./object_detection_yolo.out --video=../data/videos/run_sm.mp4}"
				   "{image i        |<none>| input image   }"
				   "{device i        |<none>| camera device   }"
				   "{video v       |<none>| input video   }";

typedef struct MyParser
{
	string InputFile;
	string OutputFile;
	bool IsVideoOutput;
	int Parse(int argc, char **argv)
	{
		CommandLineParser parser(argc, argv, keys);
		parser.about("Use this script to run object detection using YOLO3 in OpenCV.");
		if (parser.has("help"))
		{
			parser.printMessage();
			return 0;
		}

		try
		{
			string file;
			if (parser.has("image"))
			{
				IsVideoOutput = false;
				file = parser.get<string>("image");
				InputFile = file;
				OutputFile = file.replace(file.end() - 4, file.end(), "_output.jpg");
			}
			else if (parser.has("video"))
			{
				IsVideoOutput = true;
				file = parser.get<string>("video");
				InputFile = file;
				OutputFile = file.replace(file.end() - 4, file.end(), "_output.avi");
			}
			else
			{
				IsVideoOutput = true;
				int device = parser.get<int>("device");
				InputFile = std::to_string(device);
				OutputFile = "./camera_output.avi";
			}
		}
		catch (...)
		{
			cout << "Could not open the input image/video stream" << endl;
			return 0;
		}
	}
} MyParser_t;
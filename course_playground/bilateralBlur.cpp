#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
Mat pencilS(Mat image) {
	string filename = "C:\\Users\\leony\\source\\repos\\opencv_playground\\images\\trump.jpg";// DATA_PATH + "images/trump.jpg";

	// Read the image

	//imshow("image", image);
	//waitKey(0);
	Mat laplacian, LOG;
	int kernelSize = 3;
	Mat img1;
	Mat display, imageBinary;
	GaussianBlur(image, img1, Size(3, 3), 0, 0);
	// Applying laplacian
	Laplacian(img1, laplacian, CV_32F, kernelSize, 1, 0);

	// Normalize images
	//normalize(laplacian, laplacian, 1, 0, NORM_MINMAX, CV_32F);
	threshold(laplacian, imageBinary, 12, 255, THRESH_BINARY_INV);

	return imageBinary;
}
int BilateralFiltered() {
	string filename = "C:\\Users\\leony\\source\\repos\\opencv_playground\\images\\trump.jpg";// DATA_PATH + "images/trump.jpg";

	// Read the image
	Mat image = imread(filename, IMREAD_COLOR);
	Mat imageGray;
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	//diameter of the pixel neighbourhood used during filtering
	int dia = 41;
	Mat bilateralFiltered, img1, laplacian, imageBinary, imagePencil;
	Mat cartoonImage;
	bilateralFilter(image, cartoonImage, 21, 100, 20);
	bilateralFiltered = cartoonImage.clone();
	Mat pencil = pencilS(imageGray);
	pencil.convertTo(pencil, CV_8UC1);
	cartoonImage.convertTo(cartoonImage, CV_8UC3);
	Mat channels[3];
	split(cartoonImage, channels);
	for (int i = 0; i < 3; i++)
	{
		bitwise_and(channels[i], pencil, channels[i]);
	}
	merge(channels, 3, cartoonImage);

	imshow("Original Image", image);
	waitKey(0);
	imshow("Bilateral Blur Result", bilateralFiltered);
	waitKey(0);
	imshow("pencil", pencil);
	waitKey(0);
	imshow("cartoon", cartoonImage);
	waitKey(0);
	return 0;
}

#include <iostream>

#include "../resource/lib/public/includeLibraries.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "../resource/lib/public/matplotlibcpp.h"
#include "../resource/lib/public/displayImages.h"

using namespace std;

using namespace cv;

using namespace matplotlibcpp;

// Image path
string imagePath = DATA_PATH + "images/CoinsA.png";
// Read image
// Store it in the variable image
///
/// YOUR CODE HERE
///

Mat image = imread(imagePath, IMREAD_COLOR);

Mat imageCopy = image.clone();

plt::figure();
plt::imshow(image);
auto pltImg = displayImage(image);

pltImg

// Convert image to grayscale
// Store it in the variable imageGray
///
/// YOUR CODE HERE
///

Mat imageGray;
cvtColor(image, imageGray, COLOR_BGR2GRAY);

plt::figure();
plt::imshow(imageGray);
pltImg = displayImage(imageGray);
pltImg

// Split cell into channels
// Store them in variables imageB, imageG, imageR
///
/// YOUR CODE HERE
///

Mat channels[3];
split(image, channels);
Mat imageB = channels[0];
Mat imageG = channels[1];
Mat imageR = channels[2];


plt::figure_size(900,200);
plt::subplot(1,3,1);
plt::imshow(imageB);
pltImg = displayImage(imageB);
plt::subplot(1,3,2);
plt::imshow(imageG);
pltImg = displayImage(imageG);
plt::subplot(1,3,3);
plt::imshow(imageR);
pltImg = displayImage(imageR)

///
/// YOUR CODE HERE
///
Mat src = imageG;
int thresh = 30;
int maxValue = 255;

Mat dst;
threshold(src, dst, thresh, maxValue, THRESH_BINARY_INV);

// Display image using matplotlibcpp
// We have provided the sample code here
// Please modify it as required
plt::figure();
plt::imshow(dst);
pltImg = displayImage(dst);
pltImg

///
/// YOUR CODE HERE
///
int kSizeDilate = 7;
Mat kernelDilate = getStructuringElement(MORPH_ELLIPSE, Size(kSizeDilate, kSizeDilate));

///
/// YOUR CODE HERE
///
Mat imageDilated;
dilate(dst, imageDilated, kernelDilate);

// Display image using matplotlibcpp
// We have provided the sample code here
// Please modify it as required
plt::figure();
plt::imshow(imageDilated);
pltImg = displayImage(imageDilated);
pltImg


///
/// YOUR CODE HERE
///
int kSizeErode = 9;
Mat kernelErode = getStructuringElement(MORPH_ELLIPSE, Size(kSizeErode, kSizeErode));


///
/// YOUR CODE HERE
///
Mat imageEroded;
erode(imageDilated, imageEroded, kernelErode);

// Display image using matplotlibcpp
// We have provided the sample code here
// Please modify it as required
plt::figure();
plt::imshow(imageEroded);
pltImg = displayImage(imageEroded);
pltImg

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

// Detect blobs
///
/// YOUR CODE HERE
///
vector<KeyPoint> keypoints;
detector->detect(imageEroded, keypoints);

// Print number of coins detected
///
/// YOUR CODE HERE
///
cout << "number of coins detected = " << keypoints.size() << endl;

// Mark coins using image annotation concepts we have studied so far
int x,y;
int radius;
double diameter;
///
/// YOUR CODE HERE
///
for (int i = 0; i < keypoints.size(); i++)
{
    KeyPoint k = keypoints[i];
    Point keyPt;
    keyPt = k.pt;
    x = (int)keyPt.x;
    y = (int)keyPt.y;

    circle(image, Point(x, y), 5, Scalar(255, 0, 0), 10);

    diameter = k.size;
    radius = (int)diameter / 2.0;

    circle(image, Point(x, y), radius, Scalar(0, 255, 0), 2);
}

// Display image using matplotlibcpp
// We have provided the sample code here
// Please modify it as required
Mat imageRGB;
cvtColor(image, imageRGB, COLOR_BGR2RGB);
plt::figure();
plt::imshow(imageRGB);
pltImg = displayImage(imageRGB);
pltImg

Mat displayConnectedComponents(Mat &im)
{
 // Make a copy of the image
 Mat imLabels = im.clone();

 // First let's find the min and max values in imLabels
 Point minLoc, maxLoc;
 double min, max;

 // The following line finds the min and max pixel values
 // and their locations in an image.
 minMaxLoc(imLabels, &min, &max, &minLoc, &maxLoc);
 
 // Normalize the image so the min value is 0 and max value is 255.
 imLabels = 255 * (imLabels - min) / (max - min);
 
 // Convert image to 8-bits
 imLabels.convertTo(imLabels, CV_8U);
 
 // Apply a color map
 Mat imColorMap;
 applyColorMap(imLabels, imColorMap, COLORMAP_JET);

 return imColorMap;
}

// Find connected components
// Use displayConnectedComponents function provided above
///
/// YOUR CODE HERE
///
Mat imThresh;
threshold(imageG, imThresh, 30, 255, THRESH_BINARY);

int kSize3 = 7;
Mat kernel3 = getStructuringElement(MORPH_ELLIPSE, Size(kSize3, kSize3));

Mat imageMorphOpen;
morphologyEx(imThresh, imageMorphOpen, MORPH_OPEN, kernel3, Point(-1, -1), 1);

Mat imLabels;
int nComponents = connectedComponents(imageMorphOpen, imLabels);

cout << "number of connected components detected = " << nComponents << endl;


Mat colorMap = displayConnectedComponents(imLabels);


// Display image using matplotlibcpp
// We have provided the sample code here
// Please modify it as required
plt::figure();
plt::imshow(colorMap);
pltImg = displayImage(colorMap);
pltImg

// Find all contours in the image
///
/// YOUR CODE HERE
///
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;
findContours(imageMorphOpen, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);



// Print the number of contours found
///
/// YOUR CODE HERE
///
cout << "Number of contours found = " << contours.size() << endl;

// Draw all contours
///
/// YOUR CODE HERE
///
Mat imageContours = imageCopy.clone();

drawContours(imageContours, contours, -1, Scalar(0, 0, 0), 3);

plt::figure();
plt::imshow(imageContours);
pltImg = displayImage(imageContours);
pltImg


// Remove the inner contours
// Display the result
///
/// YOUR CODE HERE
///




// Print area and perimeter of all contours
///
/// YOUR CODE HERE
///
double area;
double perimeter;
double smallest_area = 1000000;
int smallest_contour_index = -1;
for (size_t i = 0; i < contours.size(); i++) {
    area = contourArea(contours[i]);
    perimeter = arcLength(contours[i], true);
    cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << endl;
    if(area < smallest_area)
    {
        smallest_area = area;
        smallest_contour_index = i;
    }
}

//the result does not have the outer box contour.
//In comparsion, the result has a contour for a small noise blob at the bottom of the image.
//i.e. contour #1 shown in the print output

// Print maximum area of contour
// This will be the box that we want to remove
///
/// YOUR CODE HERE
///
cout << "Smallest area of contour = " << smallest_area << endl;
cout << "Smallest contour # is = " << smallest_contour_index + 1 << endl;

// Remove this contour and plot others
///
/// YOUR CODE HERE
///
auto iter = contours.begin();
std::advance(iter, smallest_contour_index);
contours.erase(iter);

//the smallest contour is removed from contours vector
cout << "Number of contours left = " << contours.size() << endl;

imageContours = imageCopy.clone();


drawContours(imageContours, contours, -1, Scalar(255, 0, 0), 5);

plt::figure();
plt::imshow(imageContours);
pltImg = displayImage(imageContours);
pltImg

// Fit circles on coins
///
/// YOUR CODE HERE
///
Mat imageFitCircle = imageCopy.clone();
Point2f center;
float fitRadius;
for (size_t i = 0; i < contours.size(); i++) {
    // Fit a circle
    minEnclosingCircle(contours[i], center, fitRadius);
    circle(imageFitCircle, center, fitRadius, Scalar(0, 255, 0), 2);
    circle(imageFitCircle, center, 1, Scalar(0, 0, 255), 5);
}

plt::figure();
plt::imshow(imageFitCircle);
pltImg = displayImage(imageFitCircle);
pltImg




// Image path
imagePath = DATA_PATH + "images/CoinsB.png";
// Read image
// Store it in variable image
///
/// YOUR CODE HERE
///
image = imread(imagePath, IMREAD_COLOR);
imageCopy = image.clone();

// Convert image to grayscale
// Store it in the variable imageGray
///
/// YOUR CODE HERE
///

cvtColor(image, imageGray, COLOR_BGR2GRAY);

// Display image
plt::figure();
plt::imshow(imageGray);
pltImg = displayImage(imageGray);
pltImg

// Split cell into channels
// Store them in variables imageB, imageG, imageR
///
/// YOUR CODE HERE
///
split(image, channels);
imageB = channels[0];
imageG = channels[1];
imageR = channels[2];

plt::figure_size(1100,200);
plt::subplot(1,3,1);
plt::imshow(imageB);
pltImg = displayImage(imageB);
plt::subplot(1,3,2);
plt::imshow(imageG);
pltImg = displayImage(imageG);
plt::subplot(1,3,3);
plt::imshow(imageR);
pltImg = displayImage(imageR)

///
/// YOUR CODE HERE
///
src = imageGray;
thresh = 150;
maxValue = 255;

threshold(src, imThresh, thresh, maxValue, THRESH_BINARY);

dst = imThresh.clone();

plt::figure();
plt::imshow(dst);
pltImg = displayImage(dst);
pltImg

///
/// YOUR CODE HERE
///
int kSize = 3;
Mat element;
Mat imageMorphClosed;


element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize, kSize));
morphologyEx(imThresh, imageMorphClosed, MORPH_CLOSE, element, Point(-1,-1), 1);
plt::figure();
plt::imshow(imageMorphClosed);
pltImg = displayImage(imageMorphClosed);
pltImg

///
/// YOUR CODE HERE
///

element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize, kSize));
morphologyEx(imThresh, imageMorphClosed, MORPH_CLOSE, element, Point(-1,-1), 5);
plt::figure();
plt::imshow(imageMorphClosed);
pltImg = displayImage(imageMorphClosed);
pltImg

///
/// YOUR CODE HERE
///

kSize = 9;
element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize, kSize));

morphologyEx(imageMorphClosed, imageMorphOpen, MORPH_OPEN, element, Point(-1, -1), 1);
plt::figure();
plt::imshow(imageMorphOpen);
pltImg = displayImage(imageMorphOpen);
pltImg

///
/// YOUR CODE HERE
///
kSize = 9;
element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize, kSize));

morphologyEx(imageMorphClosed, imageMorphOpen, MORPH_OPEN, element, Point(-1, -1),6);
plt::figure();
plt::imshow(imageMorphOpen);
pltImg = displayImage(imageMorphOpen);
pltImg

///
/// YOUR CODE HERE
///
kSize = 21;
element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize, kSize));

morphologyEx(imageMorphClosed, imageMorphOpen, MORPH_OPEN, element, Point(-1, -1), 6);
plt::figure();
plt::imshow(imageMorphOpen);
pltImg = displayImage(imageMorphOpen);
pltImg

///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


///
/// YOUR CODE HERE
///


// Setup SimpleBlobDetector parameters.

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
detector = SimpleBlobDetector::create(params);

// Detect blobs
///
/// YOUR CODE HERE
///

detector->detect(imageMorphOpen, keypoints);


// Print number of coins detected
///
/// YOUR CODE HERE
///
cout << "number of coins detected: " << keypoints.size() << endl;


// Mark coins using image annotation concepts we have studied so far
///
/// YOUR CODE HERE
///
image = imageCopy.clone();

for (int i = 0; i < keypoints.size(); i++) 
{
    KeyPoint k = keypoints[i];
    Point keyPt;
    keyPt = k.pt;
    x = (int)keyPt.x;
    y = (int)keyPt.y;

    circle(image, Point(x, y), 5, Scalar(255, 0, 0), -1);

    diameter = k.size;
    radius = (int)diameter / 2.0;

    circle(image, Point(x, y), radius, Scalar(0, 0, 255), 10);
}

///
/// YOUR CODE HERE
///
plt::figure();
plt::imshow(image);
pltImg = displayImage(image);
pltImg

// # Find connected components
///
/// YOUR CODE HERE
///
Mat imageBinary;
bitwise_not(imageMorphOpen, imageBinary);


kSize = 21;
element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize, kSize));
Mat binaryOpen;
morphologyEx(imageBinary, binaryOpen, MORPH_OPEN, element, Point(-1, -1), 3);


imageBinary = binaryOpen.clone();


nComponents = connectedComponents(imageBinary, imLabels);
cout << "number of connected components: " << nComponents << endl;

colorMap = displayConnectedComponents(imLabels);


///
/// YOUR CODE HERE
///
plt::figure();
plt::imshow(colorMap);
pltImg = displayImage(colorMap);
pltImg

// Find all contours in the image
///
/// YOUR CODE HERE
///
findContours(imageBinary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);



// Print the number of contours found
///
/// YOUR CODE HERE
///
cout << "Number of contours found = " << contours.size() << endl;

// Draw all contours
///
/// YOUR CODE HERE
///
imageContours = imageCopy.clone();
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

plt::figure();
plt::imshow(imageContours);
pltImg = displayImage(imageContours);
pltImg

// Remove the inner contours
// Display the result
///
/// YOUR CODE HERE
///

//the result does not have extra contour to remove



// Print area and perimeter of all contours
///
/// YOUR CODE HERE
///
for (size_t i = 0; i < contours.size(); i++)
{
    area = contourArea(contours[i]);
    perimeter = arcLength(contours[i], true);
    cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << endl;
}

// Print maximum area of contour
// This will be the box that we want to remove
///
/// YOUR CODE HERE
///


// Remove this contour and plot others
///
/// YOUR CODE HERE
///


// Print area and perimeter of all contours
///
/// YOUR CODE HERE
///


// Remove contours
///
/// YOUR CODE HERE
///


// Draw revised contours
///
/// YOUR CODE HERE
///


// Fit circles on coins
///
/// YOUR CODE HERE
///
imageFitCircle = imageCopy.clone();

for (size_t i = 0; i < contours.size(); i++) 
{
    // Fit a circle
    minEnclosingCircle(contours[i], center, fitRadius);
    circle(imageFitCircle, center, fitRadius, Scalar(255, 0, 0), 10);
    circle(imageFitCircle, center, 5, Scalar(0, 0, 255), 10);
}

plt::figure();
plt::imshow(imageFitCircle);
pltImg = displayImage(imageFitCircle);
pltImg



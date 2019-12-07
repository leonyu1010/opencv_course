#include "getRandomColors.h"
using namespace std;
using namespace cv;
// Fill the vector with random colors
void getRandomColors(vector<Scalar> &colors, int numColors)
{
	RNG rng(0);
	for (int i = 0; i < numColors; i++)
		colors.push_back(Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
}
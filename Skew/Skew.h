#ifndef SKEW_H_
#define SKEW_H_

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class Skew {
public:
	Skew();

	/**
	    Finds image's threshold based on a specific percentage
	 	specified by one of the parameters

	    @param Percentage to control threshold's accumulator.
	    @param Target image.
	    @return ideal threshold for preprocess.
	*/

	int findThreshold(float percentage, Mat src);

	/**
	    Calculates the mode of a vector of numbers.

	    @param Vector of numbers.
	    @return Mode of a vector of numbers.
	*/
	double findMode(vector<double> array);

	/**
	    Calculates the mean of a vector of numbers.

	    @param Vector of numbers.
	    @return Mean of a vector of numbers.
	*/
	double findMean(vector<double> array);

	/**
	    Loads a image by it's file path and reprocesses it.

	    @param File path.
	    @return Image after preprocess.
	*/
	Mat preprocess(String filename);

	/**
	    Rotates a image in a specific angle.

	    @param Target image.
	    @param Angle of rotation.
	    @return Rotated image.
	*/
	Mat rotateImage(Mat img, double angle);

	/**
	    Finds necessary angle for the image orientation normalization.

	    @param File path.
	    @return Angle of rotation.
	*/
	double computeSkew(String filename, int i);

	void deskew(String filename, double angle);


	virtual ~Skew();
};

#endif /* SKEW_H_ */

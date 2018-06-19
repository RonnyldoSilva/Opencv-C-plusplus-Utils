#include <math.h>
#include <iostream>
#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    
	Mat rgb = imread(argv[1],CV_LOAD_IMAGE_COLOR);

	Mat gray;
	Mat xyz;
	Mat ycrcb;
	Mat hsv;
	Mat hls;

	cvtColor(rgb, gray, cv::COLOR_RGB2GRAY);
	cvtColor(rgb, xyz, cv::COLOR_RGB2XYZ);
	cvtColor(rgb, ycrcb, cv::COLOR_RGB2YCrCb);
	cvtColor(rgb, hsv, cv::COLOR_RGB2HSV);
	cvtColor(rgb, hls, cv::COLOR_RGB2HLS);
	
	imwrite("1_gray.jpg", gray);
	imwrite("2_xyz.jpg", xyz);
	imwrite("3_ycrcb.jpg", ycrcb);
	imwrite("4_hsv.jpg", hsv);
	imwrite("5_hls.jpg", hls);

	return 0;
}

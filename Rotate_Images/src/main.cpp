#include <iostream>
#include <opencv2/opencv.hpp>
#include "InputOutput.h"
#include "ImagePreProcess.h"

using namespace std;
using namespace cv;

int main (int argc, char** argv) {
	
	std::string input_dir = argv[1];	
	std::vector<std::string> img_names = InputOutput::getName(input_dir);		
	
	for(int i = 0 ; i < img_names.size(); ++i) {

		cv::Mat img = cv::imread(input_dir + "/" + img_names[i]);

		cv::Mat rotated_img = ImagePreProcess::rotateImage(img);

		stringstream ss;
		ss << i;
		string str = ss.str();

		cv::imwrite("images_rotated/" + str + ".jpg", rotated_img);
	}

	return 0;
}

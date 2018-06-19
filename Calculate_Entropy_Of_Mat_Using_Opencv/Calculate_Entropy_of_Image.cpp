#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double calcEntropy(Mat frame) {

	map<unsigned char, size_t> pixel_map;

	for (int j = 0; j < frame.rows; j++) {

		for (int i = 0; i < frame.cols; i++) {

			pixel_map[frame.at<unsigned char>(j, i)]++;
		}
	}

	double entropy = 0;

	for (map<unsigned char, size_t>::const_iterator ci = pixel_map.begin(); ci != pixel_map.end(); ci++) {

		double probability = ci->second / static_cast<double>(frame.rows*frame.cols);
		entropy += probability * log(probability);
	}

	entropy = -entropy;

	return entropy;
}

int main (int argc, const char* argv[]) {

	cv::Mat src = cv::imread(argv[1]);

	if(!src.data) {
	
		std::cerr << "Problem loading image!!!" << std::endl;
	}

	cv::Mat gray;
	if (src.channels() == 3) {

		cv::cvtColor(src, gray, CV_BGR2GRAY);
	}
	else {

		gray = src;
	}

	double entropy = calcEntropy(gray);

	cout << "Entropy: " << entropy << endl;
	
	return 0;
}

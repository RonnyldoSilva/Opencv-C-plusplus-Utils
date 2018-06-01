#include <limits>
#include "Skew.h"
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <bits/stdc++.h>

#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::text;

Skew::Skew() {}

Mat Skew::rotateImage(Mat img, double angle){
	Mat imgRotated;
	Mat matRotation = getRotationMatrix2D(Point(img.size().width/2, img.size().height/2),angle,1);
	warpAffine(img,imgRotated,matRotation,img.size(),INTER_LANCZOS4,false,Scalar(255,255,255));

	return imgRotated;
}

int Skew::findThreshold(float percentage, Mat src){
	float range[] = { 0, 255 };
	const float *ranges[] = { range };

	MatND hist;
	int histSize = 256;
	calcHist( &src, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );

	float sumOfBins = 0;
	float threshold = 0;

	for (int i = 0; i < histSize; ++i){
		sumOfBins += hist.at<float>(i);
	}

	for (int i = 0; i < histSize; ++i){
		threshold += hist.at<float>(i);
		if(percentage<threshold/sumOfBins){
			threshold = i;
			break;
		}
	}
	return threshold;
}

double Skew::findMean(vector<double> array){
	double sum = 0;
	for (int i =0;i<array.size();i++){
		sum+= array[i];
	}
	return sum/array.size();
}

double Skew::findMode(vector<double> array){
	sort(array.begin(), array.end());

	int currentNumIndex = 0;
	int max = 0;

	vector<double> modeNums;
	vector<double> currentNums;
	currentNums.push_back(array[0]);

	for (int i = 0; i<array.size(); i++){
		int diff = abs(array[currentNumIndex]-array[i]);
		if(diff<2){
			currentNums.push_back(array[i]);
			int size = currentNums.size();

			if(size>max){
				max = size;
				modeNums = currentNums;
			}
		}
		else{
			currentNumIndex = i;
			currentNums.clear();
			currentNums.push_back(array[i]);
		}
	}
	return findMean(modeNums);
}

Mat Skew::preprocess(String filename){
	double alpha = 0.8;
	double beta = 1;

	Mat modifiedImage = cv::imread(filename, 0);

	medianBlur(modifiedImage,modifiedImage,3);

	//MODIFY CONTRAST
	modifiedImage.convertTo(modifiedImage, -1, alpha, beta);

	int thres = findThreshold(0.08, modifiedImage);
	std::cout << "threshold : " << thres << std::endl;

	adaptiveThreshold(modifiedImage, modifiedImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 5, 6);
	return modifiedImage;
}

String intToString (int a){
	ostringstream temp;
	temp<<a;
	return temp.str();
}


double Skew::computeSkew(String filename,int index){

	Mat modifiedImage = preprocess(filename);

	cv::Size size = modifiedImage.size();

	cv::namedWindow("PRE-PROCESSAMENTO", cv::WINDOW_NORMAL);
	cv::resizeWindow("PRE-PROCESSAMENTO", 600, 600);
	cv::imshow("PRE-PROCESSAMENTO", modifiedImage);

	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(modifiedImage, lines, 1, CV_PI/180, 10, size.width / 2.5, 20);

	cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));

	double angleOfRotation = 0.;

	int numLines = lines.size();
	vector<double> angles;

	const double radToDegree = 180 / CV_PI;

	for (int i = 0; i < numLines; i++){
		cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]),
				cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0 ,0));

		double angle = atan2((double)lines[i][3] - lines[i][1],
				(double)lines[i][2] - lines[i][0])*radToDegree;

		if(angle!=0){
			angles.push_back(angle);
		}
	}

	if(angles.size()>0){
		angleOfRotation = findMode(angles);
	}

	if(angleOfRotation>50){
		angleOfRotation = angleOfRotation-90;
	}
	else if(angleOfRotation<-50){
		angleOfRotation = 90+angleOfRotation;
	}

	std::cout << "File " << filename << ": " << angleOfRotation << std::endl;

//RESULTS BEFORE AND AFTER ROTATION
	Mat originalImage = cv::imread(filename, 1);

	Mat imgRotated = rotateImage(originalImage, angleOfRotation);

	string imgPath = "Images/paraOCR/Rotacionadas/cv" + intToString(index) + ".jpeg";
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_JPEG_QUALITY );
	imwrite(imgPath,imgRotated,compression_params);

//
//	cv::namedWindow(filename, WINDOW_NORMAL);
//	cv::resizeWindow(filename, 600, 600);
//	cv::imshow(filename ,originalImage);
//
//	cv::namedWindow("RESULT", cv::WINDOW_NORMAL);
//	cv::resizeWindow("RESULT", 600, 600);
//	cv::imshow("RESULT",imgRotated);

//	cv::namedWindow("ACCUMULATOR", cv::WINDOW_NORMAL);
//	cv::resizeWindow("ACCUMULATOR", 600, 600);
//	cv::imshow("ACCUMULATOR", disp_lines);
//
//	cv::waitKey(0);
//	cv::destroyWindow(filename);

	return angleOfRotation;
}

Skew::~Skew() {
	// TODO Auto-generated destructor stub
}


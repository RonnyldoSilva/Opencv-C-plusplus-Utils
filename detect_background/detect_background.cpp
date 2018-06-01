#include "detect_background.h"
#include <vector>
#include <numeric>
#include <string>
#include <cstring>
#include <functional>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

detectarFundo::detectarFundo() {}

const int minTomDeCinzaDefault = 110;
const int maxTomDeCinzaDefault = 160;
const int espessuraBorda = 5;

Mat detectarFundo::calcularHistograma(Mat img){
	float range[] = { 0, 255 };
	const float *ranges[] = { range };
	Mat branco = imread("Images/33.jpg",0);
	Mat hist;
	int histSize = 256;
	calcHist( &img, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );
	return hist.t();
}

//Mat detectarFundo::somaDeHistogramas(vector<Mat> hists){
//	Mat acum = cv::Mat::zeros(1, 256, CV_32FC1);
//
//	for (int i = 0; i < hists.size(); i++)
//	{
//		for(int j= 0;j<hists[i].cols; j++)
//		{
//			acum.at<float>(0, j) += hists[i].at<float>(0, j);
//		}
//	}
//	return acum;
//}

vector<int> detectarFundo::calcularPicosHistogramaBordas(Mat img){
	Mat top = img(Rect(0, 0, img.size().width, espessuraBorda));
	Mat bottom = img(Rect(0, img.size().height-espessuraBorda, img.size().width, espessuraBorda));
	Mat left = img(Rect(0, espessuraBorda,espessuraBorda , img.size().height-(2*espessuraBorda)));
	Mat right= img(Rect(img.size().width-espessuraBorda, espessuraBorda, espessuraBorda, img.size().height-2*espessuraBorda));

	vector<Mat> imgs;
	vector<int> picos;

	imgs.push_back(top);
	imgs.push_back(bottom);
	imgs.push_back(left);
	imgs.push_back(right);

	double min,max;
	Point minIndex,maxIndex;

	for (int i=0; i<imgs.size();i++){
		Mat hist = calcularHistograma(imgs[i]);
		minMaxLoc(hist,&min,&max,&minIndex,&maxIndex);
		picos.push_back(maxIndex.x);
	}
	return picos;
}

bool detectarFundo::temFundo(string imgPath){
	int qntDeHistDentroDoPico = 0;
	Mat img = imread(imgPath,0);

	vector<int> picos = calcularPicosHistogramaBordas(img);
	for(int i = 0;i<picos.size();i++){
		if(picos[i]<=maxTomDeCinzaDefault and picos[i]>=minTomDeCinzaDefault){
			qntDeHistDentroDoPico++;
		}
	}
	if(qntDeHistDentroDoPico>1){
		return true;
	}
	else{
		return false;
	}
}

detectarFundo::~detectarFundo() {
	// TODO Auto-generated destructor stub
}

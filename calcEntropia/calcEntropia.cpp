#include <limits>
#include "calcEntropia.h"
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

calcEntropia::calcEntropia() {}

double calcEntropia::calcularEntropia(Mat img)
{
	double entropy = 0;
	Mat hist = calcularHistograma(img);

	for (int x = 0; x < hist.cols; x++)
	{
		double probability = (hist.at<float>(0, x) / (double) img.total());

		if (probability != 0) {

			entropy += (probability * log2(probability));
		}
	}

	return -entropy;
}

int calcEntropia::getFrequenciaDeNum(vector<double> v, double min, double max)
{
	int freq = 0;
	for(size_t i=0;i<v.size();i++){
		if(v[i]>=min && v[i]<max){
			freq++;
		}
	}
	return freq;
}

double calcEntropia::calcularEntropiaVec(vector<double> v)
{
	double entropy = 0;
	const int num_bins = 256;
	int bins[num_bins];

	double max = *max_element(v.begin(), v.end());
	double min = *min_element(v.begin(), v.end());
	double delta = (max-min)/num_bins;

	for(int i =0;i<num_bins+1;i++)
	{
		bins[i] = getFrequenciaDeNum(v,min+(delta*i),min+(delta*(i+1)));
	}

	for (int x = 0; x < num_bins+1; x++)
	{
		double probability = (bins[x] / (double) v.size());

		if (probability != 0) {

			entropy += (probability * log2(probability));
		}
	}
	return -entropy;
}

Mat calcEntropia::calcularHistograma(Mat img)
{
	Mat hist = cv::Mat::zeros(1, 256, CV_32FC1);

	for(int i = 0 ; i < img.rows; ++i)
	{
		for(int j = 0 ; j < img.cols; ++j)
		{
			int index = int(img.at<uchar>(i,j));

			hist.at<float>(0, index)++;
		}
	}
	return hist;
}

double calcEntropia::getMaiorIntensidade(Mat img)
{
	Mat hist = calcularHistograma(img);

	double max = 0;

	for(int j = hist.cols-1;j>=0; j--)
	{
		int qnt = hist.at<float>(0, j);

		if(qnt>0){
			max = j;
			break;
		}
	}
	return max;
}

Mat calcEntropia::normalizarImg(Mat img)
{
	double max = getMaiorIntensidade(img);
	img.convertTo(img,CV_32FC1);
	for (int i = 0; i<img.rows; i++){
		for(int j=0;j<img.cols; j++){
			if( (img.at<float>(i, j) / max) >= 0.5)
			{
				img.at<float>(i,j) = 1;
			}
			else
			{
				img.at<float>(i,j) = 0;
			}
		}
	}
	return img;
}

vector<double> calcEntropia::normalizarVetor(vector<double> v)
{
	vector<double> vNormalizado;

	double max = *max_element(v.begin(), v.end());
	double min = *min_element(v.begin(), v.end());

	if(abs(min)>max){
		max = abs(min);
	}

	for(size_t i=0;i<v.size();i++){
		vNormalizado.push_back(v[i]/max);
	}

	return vNormalizado;
}



double calcEntropia::calcularEntropiaFinal(String path)
{
	Mat bgr[3];
	const double MAX_i = 5;
	const double MAX_j = 4;
	const double DIMENSAO = 224;

	Mat img = imread(path,1);
//	Mat img = imread("Imagens/teste/a.jpg",1);
	if(!img.data){
		return -1;
	}
//	cout<<"Entropia da img orgiginal: "<<calcularEntropia(img)<<endl;

	Size size(DIMENSAO,DIMENSAO);
	resize(img,img,size);

//	namedWindow("ANTES", cv::WINDOW_NORMAL);
//	resizeWindow("ANTES", 600, 600);
//	imshow("ANTES", img);
//	waitKey(0);

	int channels = img.channels();

	if(channels==3){
		split(img,bgr);
	}
	else{
		bgr[0] = img;
	}

	vector<double> entropias;

	for (int k = 0; k<channels;k++){
		for(int i = 0; i<MAX_i; i++){
			for(int j = 0; j<MAX_j; j++){
				Mat crop = bgr[k](Rect(j*(DIMENSAO/MAX_j), i*(DIMENSAO/MAX_i), DIMENSAO/MAX_j, DIMENSAO/MAX_i));

//				namedWindow("DEPOIS", cv::WINDOW_NORMAL);
//				resizeWindow("DEPOIS", 600, 600);
//				imshow("DEPOIS", crop);
//				waitKey(0);

				Mat cropNormalizado = normalizarImg(crop);

				double entropia = calcularEntropia(cropNormalizado);
				entropias.push_back(entropia);
			}
		}
	}
	vector<double> entropiasNorm = normalizarVetor(entropias);
	double entropiaFinal = pow(calcularEntropiaVec(entropiasNorm),2);

	cout<<path<<": "<<entropiaFinal<<endl;
	return entropiaFinal;
}

calcEntropia::~calcEntropia() {

}

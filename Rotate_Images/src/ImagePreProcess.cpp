#include "ImagePreProcess.h"

#include <limits>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include <limits.h>
#include <algorithm>

#include <iostream>

using namespace std;
using namespace cv;

int DELAY_CAPTION = 1000;
int DELAY_BLUR = 1000;
int MAX_KERNEL_LENGTH = 9;

Mat src; Mat dst;
char window_name[] = "Filter Demo 1";

int display_caption( char* caption );
int display_dst( int delay );
Mat teste_filters(Mat img);

ImagePreProcess::ImagePreProcess() {
	// TODO Auto-generated constructor stub

}

std::vector<int> compute_histogram( cv::Mat img ) {

	std::vector<int> hist (256);

	for ( size_t i = 0; i < img.rows; ++i ) {
		for ( size_t j = 0 ; j < img.cols; ++j ) {

			int index = img.at<uchar>( i, j );
			hist[index]++;
		}
	}

	return hist;
}

int compute_max_intensity( std::vector<int> hist ) {

	int intensity = -1;
	int max_value = -1;

	for ( size_t i = 0; i < hist.size(); ++i )	{

		if ( hist[i] > max_value ) {

			max_value = hist[i];
			intensity = i;
		}
	}

	return intensity;
}



cv::Mat ImagePreProcess::image_binarization( cv::Mat img ) {

	if (!img.data) {

		throw std::invalid_argument("Received image without data");
	}

	if (img.channels() == 1)	{
		
		throw std::invalid_argument("A imagem não é colorida, favor passar uma imagem com os canais RGB");
	}

	cv::Mat otsu;
	cv::Mat gray;
	cv::cvtColor(img, gray, CV_BGR2GRAY);

	cv::threshold(gray, otsu, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cv::Mat otsu_morpho;

	double n = 1;

	cv::morphologyEx(otsu, otsu_morpho, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * n + 1, 2 * n+ 1), cv::Point(-1, -1)), cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
	otsu_morpho  = (otsu_morpho == 255);

	std::vector<int> hist = compute_histogram(otsu_morpho);
	int intensity = compute_max_intensity(hist);

	return otsu_morpho;
}

cv::Mat teste_filters(cv::Mat imgg){

	namedWindow( window_name, CV_WINDOW_AUTOSIZE );

	src = imgg;
	dst = src.clone();

	/// Applying Homogeneous blur
	if( display_caption( "Homogeneous Blur" ) != 0 ) { 
		
		return dst;
	}
	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 ) { 
	
		blur( src, dst, Size( i, i ), Point(-1,-1) );
		if( display_dst( DELAY_BLUR ) != 0 ) {
		
			return dst;
		}
	}

	/// Applying Gaussian blur
	if( display_caption( "Gaussian Blur" ) != 0 ) { return dst; }

	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
	{ GaussianBlur( src, dst, Size( i, i ), 0, 0 );
	if( display_dst( DELAY_BLUR ) != 0 ) { return dst; } }

	/// Applying Median blur
	if( display_caption( "Median Blur" ) != 0 ) { return dst; }

	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
	{ medianBlur ( src, dst, i );
	if( display_dst( DELAY_BLUR ) != 0 ) { return dst; } }

	/// Applying Bilateral Filter
	if( display_caption( "Bilateral Blur" ) != 0 ) { return dst; }

	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
	{ bilateralFilter ( src, dst, i, i*2, i/2 );
	if( display_dst( DELAY_BLUR ) != 0 ) { return dst; } }

	/// Wait until user press a key
	display_caption( "End: Press a key!" );

	waitKey(0);
	return dst;
}

int display_caption( char* caption )
	{
	dst = Mat::zeros( src.size(), src.type() );
	putText( dst, caption,
					 Point( src.cols/4, src.rows/2),
					 CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );

	imshow( window_name, dst );
	int c = waitKey( DELAY_CAPTION );
	if( c >= 0 ) { return -1; }
	return 0;
}

int display_dst( int delay )
{
 imshow( window_name, dst );
 int c = waitKey ( delay );
 if( c >= 0 ) { return -1; }
 return 0;
}


std::pair<std::vector<cv::Mat>, std::vector<cv::Rect> > ImagePreProcess::filterText(const cv::Mat &input, std::string mode)
{
	if(!input.data)
	{
		throw std::invalid_argument("Received image without data");
	}

	std::pair<std::vector<cv::Mat>, std::vector<cv::Rect> > result;
	cv::Mat rgb;// = input;
	input.copyTo(rgb);

	cv::Mat rgbCopy;
	rgb.copyTo(rgbCopy);

	cv::Mat small;
	cv::cvtColor(rgb, small, CV_BGR2GRAY);

	// morphological gradient
	// Para linha, colocar 9, 2
	// Para bloco, colocar 2, 9
	cv::Mat morphKernel;
	if(mode == "line")
	{
		morphKernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 2));
	}
	else
	{
		morphKernel = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 9));
	}

	cv::Mat grad;
	cv::morphologyEx(small, grad, cv::MORPH_GRADIENT, morphKernel);

	// binarize
	cv::Mat bw;
	cv::threshold(grad, bw, 0.0, 255.0, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// connect horizontally oriented regions
	cv::Mat connected;

	morphKernel = getStructuringElement(cv::MORPH_RECT, cv::Size(20, 1));
	cv::morphologyEx(bw, connected, cv::MORPH_CLOSE, morphKernel);

	// find contours
	cv::Mat mask = cv::Mat::zeros(bw.size(), CV_8UC1);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(connected, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
//	cv::findContours(connected, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		cv::Rect rect = boundingRect(contours[idx]);
		cv::Mat maskROI(mask, rect);
		maskROI = cv::Scalar(0, 0, 0);
		// fill the contour
		cv::drawContours(mask, contours, idx, cv::Scalar(255, 51, 230), CV_FILLED);
		// ratio of non-zero pixels in the filled region
		double r = (double)countNonZero(maskROI)/(rect.width*rect.height);

		if (r > .40 && (rect.height > 10 && rect.width > 10)) {
			/* "r > .45" assume at least 45% of the area is filled if it contains text constraints on region size */
			/* "(rect.height > 8 && rect.width > 8)" these two conditions alone are not very robust. better to use something like the number of significant peaks in a horizontal projection as a third condition */
			cv::Mat croppedImage = rgbCopy(rect);

			int sizeCropped = croppedImage.total();
			int sizeSrc = rgb.total();
			double limitCropped = sizeSrc * 0.8;

			if (sizeCropped <= limitCropped) {

				result.first.push_back(croppedImage);
				result.second.push_back(rect);
			}
		}
	}


	return result;
}

int max(int a, int b)
{
	return a>b?a:b;
}
int min(int a, int b)
{
	return a<b?a:b;
}

cv::Mat ImagePreProcess::segment_image_by_text_blocks(std::vector<cv::Rect> v_blocks, cv::Mat img)
{
	if(!img.data)
	{
		throw std::invalid_argument("Received image without data");
	}

	if(v_blocks.size() == 0) return img;

	cv::Mat image_segmented;

	int min_x = 99999999;
	int min_y = 99999999;

	int max_x = -99999999;
	int max_y = -99999999;

	for (size_t i = 0; i < v_blocks.size(); ++i)
	{
		if(v_blocks[i].x < min_x) min_x = v_blocks[i].x;
		if(v_blocks[i].x + v_blocks[i].width > max_x) max_x = v_blocks[i].x + v_blocks[i].width;

		if(v_blocks[i].y < min_y) min_y = v_blocks[i].y;
		if(v_blocks[i].y + v_blocks[i].height > max_y) max_y = v_blocks[i].y + v_blocks[i].height;
	}

	int border = 30;
	min_x = max( min_x - border, 0);
	min_y = max( min_y - border, 0);
	max_x = min( max_x + border, img.cols);
	max_y = min( max_y + border, img.rows);

	cv::Rect r = cv::Rect(min_x, min_y, max_x-min_x, max_y-min_y);

	if(r.area() < img.rows*img.cols*0.5) return img;

	image_segmented = img(r);

	return image_segmented;
}

std::vector<cv::Mat> ImagePreProcess::split_image(cv::Mat img)
{
	std::vector<cv::Mat> crops;
	int h = img.rows/3;
	for (size_t i = 0 ; i < img.rows-h; i+=h)
	{

		int y =      max(i-30, 0);
		int height = min(img.rows, y + h + 30);

		cv::Rect r = cv::Rect(0, y, img.cols, height-y);
		cv::Mat crop = img(r);
		crops.push_back(crop);
	}

	return crops;
}

cv::Mat ImagePreProcess::rotateImage(cv::Mat img)
{
	
	double angle;
	double threshold_angle = 2;
	cv::Mat imgRotated;

	do{
		angle = computeSkew(img);
		cv::Mat matRotation = cv::getRotationMatrix2D(cv::Point(img.size().width/2, img.size().height/2),angle,1);

		int sum_blue = 0, sum_green = 0, sum_red = 0;
		for(int i = 0; i < 5;++i)
		{
			for(int j = 0 ; j < 5; ++j)
			{
				cv::Vec3b intensity = img.at<cv::Vec3b>(0,0);
				sum_blue  += (int) intensity.val[0];
				sum_green += (int) intensity.val[1];
				sum_red   += (int) intensity.val[2];
			}
		}

		cv::warpAffine(img,imgRotated,matRotation,img.size(),cv::INTER_LANCZOS4,false, cv::Scalar(255,255,255));//)cv::Scalar(sum_blue/25,sum_green/25,sum_red/25));
		
	}while(angle <-threshold_angle && angle > threshold_angle);

	return imgRotated;
}

int ImagePreProcess::findThreshold(float percentage, cv::Mat src){
	float range[] = { 0, 255 };
	const float *ranges[] = { range };

	cv::MatND hist;
	int histSize = 256;
	cv::calcHist( &src, 1, 0, cv::Mat(), hist, 1, &histSize, ranges, true, false );

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

double ImagePreProcess::findMean(std::vector<double> array){
	double sum = 0;
	for (int i =0;i<array.size();i++){
		sum+= array[i];
	}
	return sum/array.size();
}

double ImagePreProcess::findMode(std::vector<double> array){
	std::sort(array.begin(), array.end());

	int currentNumIndex = 0;
	int max = 0;

	std::vector<double> modeNums;
	std::vector<double> currentNums;
	currentNums.push_back(array[0]);

	for (int i = 0; i < array.size(); i++){
		int diff = fabs(array[currentNumIndex]-array[i]);
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

cv::Mat ImagePreProcess::preprocess(cv::Mat img){
	double alpha = 0.8;
	double beta = 1;

	cv::Mat modifiedImage;
	cv::cvtColor(img, modifiedImage, CV_BGR2GRAY);

	medianBlur(modifiedImage,modifiedImage,3);

	//MODIFY CONTRAST
	modifiedImage.convertTo(modifiedImage, -1, alpha, beta);

	int thres = findThreshold(0.08, modifiedImage);

	cv::adaptiveThreshold(modifiedImage, modifiedImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 5, 6);

	return modifiedImage;
}


double ImagePreProcess::computeSkew(cv::Mat img){

	cv::Mat modifiedImage = preprocess(img);

	cv::Size size = modifiedImage.size();

	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(modifiedImage, lines, 1, CV_PI/180, 10, size.width / 2.5, 20);

//	cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));

	double angleOfRotation = 0.;

	int numLines = lines.size();
	std::vector<double> angles;

	const double radToDegree = 180 / CV_PI;
	for (int i = 0; i < numLines; i++){
//		cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0 ,0));

		double angle = atan2((double)lines[i][3] - lines[i][1], (double)lines[i][2] - lines[i][0])*radToDegree;

//		if(angle!=0){
			angles.push_back(angle);
//		}
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

	return angleOfRotation;
}


ImagePreProcess::~ImagePreProcess() {
	// TODO Auto-generated destructor stub
}

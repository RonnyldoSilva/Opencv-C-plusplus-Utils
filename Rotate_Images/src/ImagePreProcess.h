/*
 * ImagePreProcess.h
 *
 *  Created on: Aug 3, 2017
 *      Author: carlos
 */

#ifndef IMAGEPREPROCESS_H_
#define IMAGEPREPROCESS_H_

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
/*
 *
 */
class ImagePreProcess {
public:
	ImagePreProcess();

	static cv::Mat image_binarization(cv::Mat img);

	/**
	    Rotates a image in a specific angle.

	    @param Target image.
	    @param Angle of rotation.
	    @return Rotated image.
	*/
	static cv::Mat rotateImage(cv::Mat img);

	/**
	 * Extrai regiões que contém textos na imagem de entrada
	 * @param input imagem de entrada
	 * @param mode, modo de computar as regiões (line ou block), que é por linha ou por blocos
	 * @return retorna os recortes da imagens que contem as regioes de texto e as posições correspondentes
	 */
	static std::pair<std::vector<cv::Mat>, std::vector<cv::Rect> > filterText(const cv::Mat &input, std::string mode);

	/**
	 * Segmenta uma imagem de entrada a partir das posições das regioes que contpem texto da imagem
	 * @param v_blocks, posições das regioes de texto da imagem
	 * @param img imagem a ser segmentada
	 * @return imagem segmentada
	 */
	static cv::Mat segment_image_by_text_blocks(std::vector<cv::Rect> v_blocks, cv::Mat img);

	static std::vector<cv::Mat> split_image(cv::Mat img);


	virtual ~ImagePreProcess();

private:

	/**
	    Finds image's threshold based on a specific percentage
	 	specified by one of the parameters

	    @param Percentage to control threshold's accumulator.
	    @param Target image.
	    @return ideal threshold for preprocess.
	*/

	static int findThreshold(float percentage, cv::Mat src);

	/**
	    Calculates the mode of a vector of numbers.

	    @param Vector of numbers.
	    @return Mode of a vector of numbers.
	*/
	static double findMode(std::vector<double> array);

	/**
	    Calculates the mean of a vector of numbers.

	    @param Vector of numbers.
	    @return Mean of a vector of numbers.
	*/
	static double findMean(std::vector<double> array);

	/**
	    Loads a image by it's file path and reprocesses it.

	    @param File path.
	    @return Image after preprocess.
	*/
	static cv::Mat preprocess(cv::Mat img);

	/**
	    Finds necessary angle for the image orientation normalization.

	    @param File path.
	    @return Angle of rotation.
	*/
	static double computeSkew(cv::Mat img);



	void deskew(std::string filename, double angle);
};

#endif /* IMAGEPREPROCESS_H_ */

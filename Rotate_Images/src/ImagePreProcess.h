#ifndef IMAGEPREPROCESS_H_
#define IMAGEPREPROCESS_H_

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/core/core.hpp>

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
		 * Extracts regions containing text in the input image
		 * @param, input image
		 * @param mode, way to compute the regions (line or block), which is by line or by blocks.
		 * @return returns the clippings of the images containing the text regions and corresponding positions.
		 */
		static std::pair<std::vector<cv::Mat>, std::vector<cv::Rect> > filterText(const cv::Mat &input, std::string mode);

		/**
		 * Segments an input image from regions positions that contain image text.

		 * @param v_blocks, positions of the text regions of the image.
		 * @param img, image to target.
		 * @return, segmented image.
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

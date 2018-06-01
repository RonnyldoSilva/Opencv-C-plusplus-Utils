#ifndef CAL_ENTROPIA_H_
#define CAL_ENTROPIA_H_

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class calcEntropia {

	public:
		calcEntropia();

		Mat calcularHistograma(Mat img);
		Mat normalizarImg(Mat img);

		vector<double> normalizarVetor(vector<double> v);

		void calcularHistograma(Mat src, int histogram[]);

		int getFrequenciaDeNum(vector<double> v, double min, double max);

		double calcularEntropiaVec(vector<double> v);
		double getMaiorIntensidade(Mat img);
		double calcularEntropia(Mat img);
		double calcularEntropiaFinal(String path);

		virtual ~calcEntropia();
};

#endif /* CAL_ENTROPIA_H_ */

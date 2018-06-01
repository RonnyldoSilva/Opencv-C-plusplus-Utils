#ifndef DETECTAR_FUNDO
#define DETECTAR_FUNDO_H_

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class detectarFundo {
public:
	detectarFundo();
	bool temFundo(string imgPath);
	Mat calcularHistograma(Mat img);
//	Mat somaDeHistogramas(vector<Mat> hists);
	vector<int> calcularPicosHistogramaBordas(Mat img);
	virtual ~detectarFundo();
};



#endif /* DETECTAR_FUNDO_H_*/

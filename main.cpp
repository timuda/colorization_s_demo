#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "InputImage.hpp"
#include "BilateralGrid.hpp"

using namespace cv;
using namespace std;

const String keys =
	"{help h usage |      | print this message   }"
	"{@image      |      | image for input   }";

int main(int argc, char **argv)
{
    String imgName;
	Mat3f mat_in;
	Mat3f mat_bg_in;
	Mat3f mat_bg_draw_in;
	
    CommandLineParser parser(argc, argv, keys);
	if(parser.has("h") || argc != 2){
		parser.printMessage();
		return 0;
	}
    imgName = parser.get<String>(0);

	mat_in= imread(imgName, 1)/255;

	//入力画像作成用のクラス
	InputImage InImg(mat_in);
	InImg.draw_Image();
	mat_bg_in = InImg.get_Image(IMG_YUV);
	mat_bg_draw_in = InImg.get_Image(IMG_DRAWYUV);

	BilateralGrid BiGr(mat_bg_in, mat_bg_draw_in);
	BiGr.construct_SliceMatrix();
	BiGr.construct_BlurMatrix();
	BiGr.calc_Bistochastic();
	BiGr.construct_AMatrix();
	BiGr.execute_ICCG(200, 0.01);

	BiGr.show_Image(BG_COLORIZED);

	imwrite("draw.png" , InImg.get_Image(IMG_DRAW)*255);
	imwrite("colorized.png" , BiGr.get_Image(BG_COLORIZED)*255);
}

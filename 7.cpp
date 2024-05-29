#include "main.h"
namespace e7 {
	void main() {
		Mat srcImg = m71, dstImg;
		cvtColor(srcImg, srcImg, COLOR_RGB2GRAY);
		distanceTransform(srcImg, dstImg, DIST_L2, 3); //欧式距离 ， 3 * 3的mask
		cout<<"\033[31m" << srcImg.size() << "\t" << dstImg.size() << "\033[0m" << endl;
		//cout<<dstImg<<endl;
		imshow("distance", dstImg);
		normalize(dstImg, dstImg, 0, 1, NORM_MINMAX);
		cout<<"\033[31m"<<dstImg.size()<<"\033[0m"<<endl;
		//resize(dstImg, dstImg, Size(dstImg.rows<<1, dstImg.cols<<1));
		imshow("normalized", dstImg);
		waitKey(0);
		return;
	}
}
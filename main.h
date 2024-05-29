#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include <iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
const string
	p11 = "../static/1.1.png",
	p12="../static/1.2.png",
	p1b="../static/1b.png",
	p1b1="../static/1b1.png",
	j21="../static/2.1.jpg",
	j61="../static/horse1.jpg",
	p62="../static/horse2.png",
	p71 = "../static/7.1.png"
;
const string outPath = "../static/out/";
const string CVdataDir = "D:\\App\\dev\\util\\opencv\\sources\\samples\\data\\";
const Mat m11 = imread(p11, IMREAD_UNCHANGED);//使能读取第4个RGBA中的A通道，即为alpha通道
const Mat m12 = imread(p12);
const Mat mb1 = imread(p1b1, IMREAD_UNCHANGED);
const Mat m21 = imread(j21);
const Mat m61 = imread(j61);
const Mat m71 = imread(p71);
extern int contrast_value, bright_value;
extern Mat tranImg;
namespace e1 {
	void main();
}
namespace e2 {
	void main();
}
namespace e3 {
	void main();
}
namespace e4 {
	void main();
}
namespace e5 {
	void main();
}
namespace e6 {
	void main();
}
namespace e7 {
	void main();
}
namespace dev {
	void main();
}
namespace test {
	void main();
}
namespace tmp {
	void main();
}

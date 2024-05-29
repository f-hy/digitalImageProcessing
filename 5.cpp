#include"main.h"

namespace e5 {
	Mat generateHistogram(vector<int> hist,int maxValue){
		int width = 512;
		int height = 512;
		Mat dst(width, height,CV_8UC1, Scalar(0));
		int n = hist.size();
		for (int i = 0; i < n; i++){
			int interval = width / n;
			int x = i * interval;
			int maxY =int( hist[i] * height / maxValue);
			for (int j = 0; j < maxY; j++)
				dst.at<uchar>(height -j-1, x)=255;
		}
		return dst;
	}
	Mat showHistogram(Mat &src, int n){
		vector<int> hist;
		int max;
		for (int k = 0; k < n; k++)
			hist.push_back(0);
		double interval = n / 256.0;
		for(int i=0;i<src.rows;i++)
		for (int j = 0; j < src.cols; j++){
			int pix = int(src.at<uchar>(i, j)*interval);
			hist[pix]++;
		}
		max = hist[0];
		for (int k = 1; k < n; k++){
			if (hist[k] > max)
				max = hist[k];
		}
		return generateHistogram(hist, max);
	}
	Mat HistogramEqualization(Mat& src, int n) {
        vector<int> hist;
        vector<double> proHist;//累计概率向量
        vector<int> dstHist;
        int max;
        double interval = n / 256.0;
        long sumPix = src.rows * src.cols;
        Mat dst(src.rows, src.cols, CV_8UC1, Scalar(0));
        for (int k = 0; k < n; k++) {
            hist.push_back(0);
            dstHist.push_back(0);
            proHist.push_back(0.0);
        }
        for (int i = 0; i < src.rows; i++)
            for (int j = 0; j < src.cols; j++)
                hist[int(src.at<uchar>(i, j) * interval)]++;
        //求出累计概率矩阵和灰度级像素数量最大值
        max = hist[0];
        proHist[0] = hist[0] * 1.0 / sumPix;
        for (int k = 1; k < n; k++) {
            proHist[k] = proHist[k - 1] + hist[k] * 1.0 / sumPix;
            if (hist[k] > max)
                max = hist[k];
        }
        //根据累计概率对灰度级进行映射
        for (int i = 0; i < n; i++) {
            dstHist[i] = int(proHist[i] * (n - 1) + 0.5);//0.5表示要四舍五入
        }
        //对图像进行映射
        for (int i = 0; i < src.rows; i++)
            for (int j = 0; j < src.cols; j++) {
                int srcLevel = int(src.at<uchar>(i, j) * interval);
                int dstLevel = dstHist[srcLevel];
                dst.at<uchar>(i, j) = int(dstLevel / interval);
            }
        return dst;
    }
	void main(){
        //see 5.ipynb
    }
}
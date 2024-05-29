#include"main.h"
namespace e3 {
    // 高斯滤波
    Mat gaussian_filter(Mat img, double sigma, int kernel_size) {
        Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);// prepare output
        // prepare kernel
        int pad = floor(kernel_size / 2); // 用于坐标系转换
        int _x = 0, _y = 0;
        double kernel_sum = 0;
        float** kernel = new float* [kernel_size];// get gaussian kernel
        for (int i = 0; i < kernel_size; ++i) kernel[i] = new float[kernel_size];
        //float kernel[kernel_size][kernel_size];
        for (int y = 0; y < kernel_size; y++) {
            for (int x = 0; x < kernel_size; x++) {
                _y = y - pad;
                _x = x - pad;
                kernel[y][x] = 1 / (2 * M_PI * sigma * sigma) * exp(-(_x * _x + _y * _y) / (2 * sigma * sigma));//二维高斯分布
                kernel_sum += kernel[y][x];
            }
        }
        for (int y = 0; y < kernel_size; y++) {// 归一化到1
            for (int x = 0; x < kernel_size; x++) {
                kernel[y][x] /= kernel_sum;
            }
        }
        for (int y = 0; y < img.rows; y++) {// filtering
            for (int x = 0; x < img.cols; x++) {
                for (int c = 0; c < img.channels(); c++) {
                    double v = 0;
                    for (int dy = -pad; dy < pad + 1; dy++) {
                        for (int dx = -pad; dx < pad + 1; dx++) {
                            int xx = x + dx;
                            int yy = y + dy;
                            // 超过边缘的就不处理了
                            if (0 <= xx && xx < img.cols && 0 <= yy && yy < img.rows) {
                                v += (double)img.ptr<Vec3b>(yy)[xx][c] * kernel[dy + pad][dx + pad];
                            }
                        }
                    }
                    out.ptr<Vec3b>(y)[x][c] = v;
                }
            }
        }
        for (int i = 0; i < kernel_size; ++i) delete[]kernel[i];
        delete[]kernel;
        return out;
    }
    // 基于行列可分离性进行加速
    Mat gaussian_filter_1D(Mat img, double sigma, int kernel_size) {
        Mat out_y = Mat::zeros(img.rows, img.cols, CV_8UC3);// y轴卷积后的结果
        Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);
        // prepare kernel
        int pad = floor(kernel_size / 2); // 用于坐标系转换
        int _x = 0, _y = 0;
        // get gaussian kernel
        float* kernel_x=new float[kernel_size];
        float* kernel_y=new float[kernel_size];
        double kernel_sum = 0;
        for (int x = 0; x < kernel_size; x++) {
            _x = x - pad;
            kernel_x[x] = 1 / (sqrt(2 * M_PI)*sigma) * exp(-(_x * _x) / (2 * sigma * sigma));
            kernel_sum += kernel_x[x];
        }
        for (int x = 0; x < kernel_size; x++) {// 归一化到1
            kernel_x[x] /= kernel_sum;
        }
        kernel_sum = 0;
        for (int y = 0; y < kernel_size; y++) {
            _y = y - pad;
            kernel_y[y] = 1 / (sqrt(2 * M_PI) * sigma) * exp(-(_y * _y) / (2 * sigma * sigma));
            kernel_sum += kernel_y[y];
        }
        for (int y = 0; y < kernel_size; y++) {// 归一化到1
            kernel_y[y] /= kernel_sum;
        }
        for (int y = 0; y < img.rows; y++) {// filtering_y
            for (int x = 0; x < img.cols; x++) {
                for (int c = 0; c < img.channels(); c++) {
                    double v = 0;
                    for (int dy = -pad; dy < pad + 1; dy++) {
                        int yy = y + dy;
                        // 超过边缘的就不处理了
                        if (0 <= yy && yy < img.rows){
                            v += (double)img.ptr<Vec3b>(y + dy)[x][c] * kernel_y[dy + pad];
                        }
                    }
                    out_y.ptr<Vec3b>(y)[x][c] = v;
                }
            }
        }
        // 这里取的是上次的结果来卷积，而不是img
        for (int y = 0; y < img.rows; y++) {// filtering_x
            for (int x = 0; x < img.cols; x++) {
                for (int c = 0; c < img.channels(); c++) {
                    double v = 0;
                    for (int dx = -pad; dx < pad + 1; dx++) {
                        int xx = x + dx;
                        // 超过边缘的就不处理了
                        if (0 <= xx && xx < img.cols)
                        {
                            v += (double)out_y.ptr<Vec3b>(y)[xx][c] * kernel_x[dx + pad];
                        }
                    }
                    out.ptr<Vec3b>(y)[x][c] = v;
                }
            }
        }
        delete[]kernel_x, kernel_y;
        return out;
    }

	void main() {
		// read image
		//Mat img = imread("../static/imori_noise.jpg", IMREAD_COLOR);
        Mat img = m21;
        imshow("raw", img);
        const int kernel_size = 7;
        clock_t start, end;
        start = clock();
		Mat out = gaussian_filter(img, 1.3, kernel_size);// gaussian filter
		end = clock();
        cout << "\033[31m高斯滤波:" << end - start << "\033[0m" << endl;
        imshow("高斯滤波", out);
        
        start = clock();
        out = gaussian_filter_1D(img, 1.3, kernel_size);
        end = clock();
        cout <<"\033[31m分离加速:" << end - start << "\033[0m" << endl;
        imshow("分离加速", out);

        start = clock();
        GaussianBlur(img, out, Size(kernel_size, kernel_size), 0);
        end = clock();
        cout << "\033[31mGaussianBlur:" << end - start << "\033[0m" << endl;
        imshow("GaussianBlur", out);
        imwrite(outPath + "GaussianBlur.png", out);
		waitKey(0);
		destroyAllWindows();
		return;
	}
}
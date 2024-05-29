#include"main.h"
namespace e1 {
    void e11() {
        vector<Mat> mask_channels;
        split(m11, mask_channels); // 分离通道，分离出 RGBA 分别作为一个图像
        imshow("alpha image", mask_channels[3]);//取出 alpha 通道
        Mat mixedImg(m11.size(), CV_8UC4);//创建8位无符号的四通道 带透明色的RGB图像
        for (int i = 0; i < m11.rows; i++)
            for (int j = 0; j < m11.cols; j++) {
                uchar A = m11.ptr<uchar>(i)[(j << 2) + 3];//srcImg.at<Vec4b>(i, j)[3];
                for (int k = 0; k < 3; k++)
                    mixedImg.at<Vec4b>(i, j)[k] = (A * m11.at<Vec4b>(i, j)[k] + (255 - A) * mb1.at<Vec4b>(i, j)[k]) / 255.0;
            }
        imshow("mixed image", mixedImg);
        waitKey(0);
    }
    static void ContrastAndBright(int pos, void* userdata) {
        for (int i = 0; i < m12.rows; i++)
            for (int j = 0; j < m12.cols; j++)
                for (int k = 0; k < 3; k++) {
                    //0<=contrast_value<=200
                    double t = ((m12.at<Vec3b>(i, j)[k] - 127) / 255.0) * contrast_value * 0.1;//[-10, 10]
                    tranImg.at<Vec3b>(i, j)[k] = m12.at<Vec3b>(i, j)[k] * ((1.0 / (1.0 + exp(-t)))) + bright_value - 100;
                }
        imshow("Display Window", m12);
        imshow("Transformed Window", tranImg);
    }
    void e12() {
        if (!m12.data)
        {
            cout << "图片读取失败！" << endl;
            exit(EXIT_FAILURE);
        }
        tranImg = Mat::zeros(m12.size(), m12.type());
        contrast_value = 20;
        bright_value = 100;
        namedWindow("Transformed Window", WINDOW_AUTOSIZE);
        createTrackbar("contrast", "Transformed Window", &contrast_value, 200, ContrastAndBright, 0);
        createTrackbar("bright", "Transformed Window", &bright_value, 200, ContrastAndBright, 0);
        waitKey(0);
    }
    void main() {
        e11();
        e12();
    }
}

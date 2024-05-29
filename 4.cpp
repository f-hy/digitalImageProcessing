#include"main.h"
namespace e4 {
    void bilateral_filter(Mat src, Mat dst, int d, double sigma_color, double sigma_space, int borderType=4) {
        int cn = src.channels();
        int i, j, k, maxk, radius;
        Size size = src.size();
        CV_Assert((src.type() == CV_8UC1 || src.type() == CV_8UC3) &&
            src.type() == dst.type() && src.size() == dst.size() &&
            src.data != dst.data);
        if (sigma_color <= 0)
            sigma_color = 1;
        if (sigma_space <= 0)
            sigma_space = 1;
        // 计算颜色域和空间域的权重的高斯核系数, 均值 μ = 0;  exp(-1/(2*sigma^2))  
        double gauss_color_coeff = -0.5 / (sigma_color * sigma_color);
        double gauss_space_coeff = -0.5 / (sigma_space * sigma_space);
        // radius 为空间域的大小: 其值是 windosw_size 的一半    
        if (d <= 0) radius = cvRound(sigma_space * 1.5);
        else radius = d / 2;
        radius = MAX(radius, 1);
        d = radius * 2 + 1;
        Mat src_border;
        copyMakeBorder(src, src_border, radius, radius, radius, radius, borderType);//添加边框
        vector<float> _color_weight(cn * 256);
        vector<float> _space_weight(d * d);
        vector<int> _space_ofs(d * d);
        float* color_weight = &_color_weight[0];
        float* space_weight = &_space_weight[0];
        int* space_ofs = &_space_ofs[0];
        // 初始化颜色相关的滤波器系数
        for (i = 0; i < 256 * cn; i++) color_weight[i] = (float)std::exp(i * i * gauss_color_coeff);
        // 初始化空间相关的滤波器系数和 offset:
        for (i = -radius, maxk = 0; i <= radius; i++) {
            j = -radius;
            for (; j <= radius; j++) {
                double r = std::sqrt((double)i * i + (double)j * j);
                if (r > radius) continue;
                space_weight[maxk] = (float)std::exp(r * r * gauss_space_coeff);
                space_ofs[maxk++] = (int)(i* src_border.step + j * cn);
            }
        }
        // 开始计算滤波后的像素值  
        for (i = 0; i < size.height; i++) {//i rows, j cols
            const uchar* sptr = src_border.ptr(i + radius) + radius * cn;  // 目标像素点 
            uchar* dptr = dst.ptr(i);
            if (cn == 1) {// 按行开始遍历
                for (j = 0; j < size.width; j++) {
                    float sum = 0, wsum = 0;
                    int val0 = sptr[j];
                    cout << endl << "计算滤波后的像素值: (" << i << "," << j << ")" << val0 << ": ";

                    // 遍历当前中心点所在的空间邻域  
                    for (k = 0; k < maxk; k++) {
                        int val = sptr[j + space_ofs[k]];
                        float w = space_weight[k] * color_weight[std::abs(val - val0)];
                        sum += val * w;
                        wsum += w;
                        cout << k << " val: " << val << " w: " << w << " sum: " << sum;
                    }
                    // 这里不可能溢出, 因此不必使用 CV_CAST_8U. 
                    dptr[j] = (uchar)cvRound(sum / wsum);
                    cout << " dst输出值：" << dptr[j] << endl;
                }
            } else {
                assert(cn == 3);
                for (j = 0; j < size.width * 3; j += 3) {
                    float sum_b = 0, sum_g = 0, sum_r = 0, wsum = 0;
                    int b0 = sptr[j], g0 = sptr[j + 1], r0 = sptr[j + 2];
                    k = 0;
                    for (; k < maxk; k++) {
                        const uchar* sptr_k = sptr + j + space_ofs[k];
                        int b = sptr_k[0], g = sptr_k[1], r = sptr_k[2];
                        float w = space_weight[k] * color_weight[std::abs(b - b0) +
                            std::abs(g - g0) + std::abs(r - r0)];
                        sum_b += b * w; sum_g += g * w; sum_r += r * w;
                        wsum += w;
                    }
                    wsum = 1.f / wsum;
                    b0 = cvRound(sum_b * wsum);
                    g0 = cvRound(sum_g * wsum);
                    r0 = cvRound(sum_r * wsum);
                    dptr[j] = (uchar)b0;
                    dptr[j + 1] = (uchar)g0;
                    dptr[j + 2] = (uchar)r0;
                }
            }
        }
    }
    void main() {
        e3::main();
        Mat img = m21;
        imshow("raw", img);
        const int kernel_size = 7;
        double sigma_color = 50, sigma_space = 3;
        clock_t start, end;
        Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);;
        
        start = clock();
        bilateral_filter(img, out, kernel_size, sigma_color, sigma_space);
        end = clock();
        cout << "\033[31mbilateral_filter:" << end - start <<"\033[0m" << endl;
        imshow("bilateral_filter", out);
        imwrite(outPath+"bilateral_filter.png", out);
        
        start = clock();
        bilateralFilter(img, out, kernel_size, sigma_color, sigma_space);
        end = clock();
        cout << "\033[31mbilateralFilter:" << end - start << "\033[0m" << endl;
        imshow("bilateralFilter", out);
        imwrite(outPath+"bilateralFilter.png", out);
        waitKey(0);
        destroyAllWindows();
        return;
    }
}
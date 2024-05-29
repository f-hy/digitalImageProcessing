#include "main.h"
namespace e6 {
	bool g_bDrawing = false;              // 是否正在绘图
	const int max_channels = 4;           // 最大通道数
	int foreground[max_channels][10000];  // 记录前景像素点的像素值
	int background[max_channels][10000];  // 记录背景像素点的像素值
	int sum_foreground[max_channels][258],
		sum_background[max_channels][258];  // 记录前景和背景像素点的像素值的个数
	int sum_foregrounds = 0, sum_backgrounds = 0;  // 记录前景和背景像素点的个数
	long long posi_foreground[10000][10000];  // 记录前景像素点的位置
	long long posi_background[10000][10000];  // 记录背景像素点的位置
	Point g_nCurrPoint, g_nCurrPointT;        // 当前鼠标位置
	/**
	 * @brief 建立鼠标响应事件，在原图像上实现画笔功能。
	 *
	 * @param event
	 * @param x
	 * @param y
	 * @param flag
	 */
	void onMouse(int event, int x, int y, int flag, void*) {
		switch (event) {
		case EVENT_MOUSEMOVE: {  // 移动鼠标的时候
			g_nCurrPointT = Point(x, y);
			break;
		}
		case EVENT_LBUTTONDOWN: {  // 点击鼠标左键时
			g_bDrawing = true;
			g_nCurrPointT = Point(x, y);
			break;
		}
		case EVENT_LBUTTONUP: {  // 松开鼠标左键时
			g_bDrawing = false;
			break;
		}
		default:
			break;
		}
	}
	// 将modify1中两个循环合并，减少循环次数，提高效率
	void getGround(const Mat srcImg, Mat& output, bool fore = true) {  // 取景
		setMouseCallback("鼠标事件窗口", onMouse, &output);  // 设置鼠标响应事件
		char key;                                            // 记录键盘按键
		int k = 0;  // 记录前景和背景像素点的个数
		while (true) {
			if (g_bDrawing) {
				circle(output, g_nCurrPointT, 0,
					fore ? Scalar(255, 0, 0) : Scalar(0, 255, 0),
					10);  // 画笔-blue,green	g_nCurrPoint:第y行第x列
				g_nCurrPoint = { g_nCurrPointT.y, g_nCurrPointT.x };
				if (posi_foreground[g_nCurrPoint.x][g_nCurrPoint.y] ==
					0) {  // 如果当前像素点没有被标记过
					//cout << "(" << g_nCurrPoint.x << ", " << g_nCurrPoint.y << ", " << srcImg.rows << "|" << srcImg.cols << "|" << srcImg.channels() << ")" << endl;
					for (int i = 0; i < output.channels(); i++) {  // 记录前景像素点的像素值
						foreground[i][k] =
							srcImg.at<Vec3b>(g_nCurrPoint.x, g_nCurrPoint.y)[i];  // modi
						sum_foreground[i]
							[foreground[i][k]]++;  // 记录前景像素点的像素值的个数
						fore ? (sum_foregrounds++) : (sum_backgrounds++);
					}
					fore ? (posi_foreground[g_nCurrPoint.x][g_nCurrPoint.y] = 1)
						: (posi_background[g_nCurrPoint.x][g_nCurrPoint.y] =
							1);  // 标记当前像素点
					k++;
				}
			}
			imshow("鼠标事件窗口", output);
			key = waitKey(2);
			if (key == '2') {  // 按下键盘2键，结束取景
				cout << k << endl;
				k = 0;
				break;
			}
			if (key == '1') {  // 按下键盘1键，重新取景
				output = srcImg;
			}
		}
	}
	void modify1(const Mat srcImg) {  // 取景
		Mat output = srcImg;            // 用于显示的图像
		Mat tempImage;                  // 用于处理的临时图像
		output.copyTo(tempImage);       // 将原图像复制到临时图像
		Mat input = srcImg;             // 用于处理的临时图像
		namedWindow("鼠标事件窗口", WINDOW_AUTOSIZE);
		getGround(srcImg, output);  // 取前景
		namedWindow("鼠标事件窗口", WINDOW_AUTOSIZE);
		getGround(srcImg, output, false);  // 取背景
	}
	void modify2(const Mat srcImg) {
		// 遍历整个图像，计算每个像素点属于前景还是背景的概率，
		// 然后根据概率将该像素点设置为白色（前景）或黑色（背景）。
		Mat output = Mat::zeros(srcImg.size(), srcImg.type());  // 用于显示的图像
		namedWindow("E4", WINDOW_AUTOSIZE);
		// 遍历全图，计算概率，看当前像素点属于前景还是背景，如果是前景就定义为白色，否则为黑色。
		for (int x = 0; x < srcImg.rows; x++) {    // 遍历每一行
			for (int y = 0; y < srcImg.cols; y++) {  // 遍历每一列
				double P_fore = 1, P_back = 1;         // 记录前景和背景的概率
				bool marked = false;  // 记录当前像素点是否被标记过
				for (int c = 0; c < srcImg.channels(); c++) {  // 遍历每个通道
					if (posi_foreground[x][y] == 1)
						output.at<Vec3b>(x, y)[c] = 255;  // 如果当前像素点属于前景，就定义为白色
					else if (posi_background[x][y] == 1)
						output.at<Vec3b>(x, y)[c] = 0;  // 如果当前像素点属于背景，就定义为黑色
					else {    // 如果当前像素点没有被标记过
						marked = true;
						double pf = 0, pb = 0;  // 记录前景和背景的像素值的概率
						int pixel_value =
							srcImg.at<Vec3b>(x, y)[c];  // 记录当前像素点的像素值
						pf = (double)sum_foreground[c][pixel_value] / (double)sum_foregrounds;
						pb = (double)sum_background[c][pixel_value] / (double)sum_backgrounds;
						if (c == 1) {// 如果当前像素点属于绿色通道
							P_fore += (0.6 * pf);
							P_back += (0.6 * pb);
						}
						else {
							P_fore += (0.2 * pf);
							P_back += (0.2 * pb);
						}
					}
				}
				if (marked == 1) {  // 如果当前像素点被标记过
					//cout<<"<"<<P_fore<<"|"<<P_back << ">" << endl;
					if (P_fore <= P_back)// 如果当前像素点属于背景，就定义为黑色
						for (int i = 0; i < srcImg.channels(); i++)
							output.at<Vec3b>(x, y)[i] = 0;
					else
						for (int i = 0; i < srcImg.channels(); i++)
							output.at<Vec3b>(x, y)[i] = 255;
				}
			}
		}
		imshow("E4", output);
	}
	void main() {
		modify1(m61);
		//compute_color_distribution(m61);
		modify2(m61);
		cout << sum_foregrounds << endl;
		int lm = 0;
		for (int i = 0; i < 256; i++) lm = lm + sum_foreground[1][i];
		cout << lm << endl;
		waitKey();
	}
}  // namespace e6
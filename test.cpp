#include"main.h"
namespace test {
    void tcamera() {
        VideoCapture capture(0);
        Mat img;
        while (true) {
            capture >> img;
            imshow("Camera", img);
            waitKey(30);
        }
    }
    void main() {
        return;
    }
}

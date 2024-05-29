#include "main.h"

int main(int argc, char* argv[])
{
    //e1::main();
    //e2::main();
    //e3::main();
    //e4::main();
    //e6::main();
    cout << "\033[31m" <<"FRID"<< cv::cuda::getCudaEnabledDeviceCount() << "\033[0m" << endl;
    Mat img = imread("D:/App/dev/util/opencv4.8.0/sources/samples/data/apple.jpg");
    if (img.empty()) {
        cout << "IMG is empty" << endl;
        return 0;
    }
    //e7::main();
    //test::main();
    //tmp::main();
    return 0;
}

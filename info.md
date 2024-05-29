[图像处理新手入门必备教程：基于OpenCV Python的数字图像处理实战，计算机博士精讲图像拼接、图像分割、图像识别、边缘检测，究极通俗易懂！_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1aL41197DC/)

### 2.1[基于OpenCV的图像透视变换详解(从理论到实现再到实践)](https://blog.csdn.net/m0_51653200/article/details/127361624)

-  仿射变换是一种二维坐标到二维坐标之间的线性变换，也就是只涉及一个平面内二维图形的线性变换。

- 保持了二维图形的两种性质
  -  “平直性”：直线经过变换之后依然是直线。一条直线经过平移、旋转、错切、放缩都还是一条直线。
  - “平行性”：变换后平行线依然是平行线，且直线上点的位置顺序不变。

- 用仿射变换的变换矩阵表示

  - 平移$\left[\matrix{1&0&T_x\\0&1&T_y}\right]\left[\matrix{x\\y\\1}\right]=\left[\matrix{x+T_x\\y+T_y}\right]$

  - 旋转$\left[\matrix{\cos\theta&-\sin\theta\\\sin\theta&\cos\theta}\right]\left[\matrix{x\\y}\right]=\left[\matrix{x\cos\theta-y\sin\theta\\x\sin\theta+y\cos\theta}\right]$

  - 错切$\left[\matrix{1&\tan\varphi\\\tan\varphi&1}\right]\left[\matrix{x\\y}\right]=\left[\matrix{x+y\tan\varphi\\y+x\tan\varphi}\right]$

  - 放缩$\left[\matrix{T_x&0\\0&T_y}\right]\left[\matrix{x\\y}\right]=\left[\matrix{T_xx\\T_yy}\right]$

  - 仿射变换可以用以下通式$\begin{equation} \left[ \begin{array}{c} x'\\ y'\\ \end{array} \right ] = \left[ \begin{array}{c} a_{11}x+a_{12}y+a_{13}\\ a_{21}x+a_{22}y+a_{23}\\ \end{array} \right ] = \left[ \begin{array}{ccc} a_{11}& a_{12}&a_{13}\\ a_{21}& a_{22} &a_{23}\\ \end{array} \right ] \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ] \end{equation}$

    坐标变换的方程组$\begin{equation} \left\{ \begin{matrix}{} x''=\frac{x'}{z'}=\frac{a_{11}x+a_{12}y+a_{13}}{a_{31}x+a_{32}y+a_{33}} \\ y''=\frac{y'}{z'}=\frac{a_{21}x+a_{22}y+a_{23}}{a_{31}x+a_{32}y+a_{33}} \\ \end{matrix} \right. \end{equation}$

- 透视变换:是二维(x,y)到三维(X,Y,Z)，再到另一个二维空间(x',y')的映射

  $\begin{equation} \left[ \begin{array}{c} x'\\ y'\\ z'\\ \end{array} \right ] = \left[ \begin{array}{ccc} a_{11}& a_{12} &a_{13}\\ a_{21}& a_{22} &a_{23}\\ a_{31}& a_{32} &a_{33}\\ \end{array} \right ] \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ] \end{equation}$

  - 仿射变换过程$\begin{equation} \left[ \begin{array}{c} x'\\ y'\\ 1\\ \end{array} \right ] = \left[ \begin{array}{ccc} a_{11}& a_{12} &a_{13}\\ a_{21}& a_{22} &a_{23}\\ 0& 0 &1\\ \end{array} \right ] \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ] \end{equation}$

  - 整个透视变换的过程$\begin{equation} \begin{aligned} \left[ \begin{array}{c} x''\\ y''\\ 1\\ \end{array} \right ] = \left[ \begin{array}{c} \frac{x'}{z'}\\ \frac{y'}{z'}\\ 1\\ \end{array} \right ] = \frac{1}{z'} \left[ \begin{array}{c} x'\\ y'\\ z'\\ \end{array} \right ] = \frac{1}{z'} \left[ \begin{array}{ccc} a_{11}& a_{12} &a_{13}\\ a_{21}& a_{22} &a_{23}\\ a_{31}& a_{32} &a_{33}\\ \end{array} \right ] \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ] \\= \frac{1}{a_{31}x+a_{32}y+a_{33}} \left[ \begin{array}{ccc} a_{11}& a_{12} &a_{13}\\ a_{21}& a_{22} &a_{23}\\ a_{31}& a_{32} &a_{33}\\ \end{array} \right ] \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ] \end{aligned} \end{equation}$

    坐标变换的方程组$\begin{equation} \left\{ \begin{matrix}{} x''=\frac{x'}{z'}=\frac{a_{11}x+a_{12}y+a_{13}}{a_{31}x+a_{32}y+a_{33}} \\ y''=\frac{y'}{z'}=\frac{a_{21}x+a_{22}y+a_{23}}{a_{31}x+a_{32}y+a_{33}} \\ \end{matrix} \right. \end{equation}$

    9个未知参数，可以进一步简化为8个未知参数$\begin{equation} \left[ \begin{array}{c} x'\\ y'\\ z'\\ \end{array} \right ] =T' \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ] =kT \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ] = \left[ \begin{array}{ccc} ka_{11}& ka_{12} &ka_{13}\\ ka_{21}& ka_{22} &ka_{23}\\ ka_{31}& ka_{32} &ka_{33}\\ \end{array} \right ] \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ] \end{equation}$

    $\begin{equation} \left\{ \begin{matrix}{} x''=\frac{k(a_{11}x+a_{12}y+a_{13})}{k(a_{31}x+a_{32}y+a_{33})}=\frac{a_{11}x+a_{12}y+a_{13}}{a_{31}x+a_{32}y+a_{33}} \\ y''=\frac{k(a_{21}x+a_{22}y+a_{23})}{k(a_{31}x+a_{32}y+a_{33})}=\frac{a_{21}x+a_{22}y+a_{23}}{a_{31}x+a_{32}y+a_{33}} \\ \end{matrix} \right. \end{equation}$

    $T= \left[ \begin{array}{ccc} b_{11}& b_{12} &b_{13}\\ b_{21}& b_{22} &b_{23}\\ b_{31}& b_{32} &1\\ \end{array} \right ]$	$\begin{equation} \left\{ \begin{matrix}{} x''=\frac{x'}{z'}=\frac{b_{11}x+b_{12}y+b_{13}}{b_{31}x+b_{32}y+1} \\ y''=\frac{y'}{z'}=\frac{b_{21}x+b_{22}y+b_{23}}{b_{31}x+b_{32}y+1} \\ \end{matrix} \right. \end{equation}$

    透视变换的过程![img](http://pic.266237.xyz/joplin/2023/09/202309251540890.png)

  - 透视变换实现

    ```cpp
    //从4对映射点计算透视变换的变换矩阵T，返回的矩阵数据类型是Mat
    Mat getPerspectiveTransform(InputArray src, InputArray dst, int solveMethod = DECOMP_LU)
    /*src：源图像四边形的4个顶点坐标。
    dst：目标图像对应四边形的4个顶点坐标。
    solveMethod：传递给cv::solve(#DecompTypes)的计算方法，默认是DECOMP_LU，一般不用输入此参数。
    返回值：Mat型变换矩阵，可直接用于warpPerspective()函数*/
    ```

    1对映射点指的是![\left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ]](https://latex.csdn.net/eq?\left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ])与![\left[ \begin{array}{c} x''\\ y''\\ 1\\ \end{array} \right ]](https://latex.csdn.net/eq?%5Cleft%5B%20%5Cbegin%7Barray%7D%7Bc%7D%20x%27%27%5C%5C%20y%27%27%5C%5C%201%5C%5C%20%5Cend%7Barray%7D%20%5Cright%20%5D)，但是变换的矩阵![T](https://latex.csdn.net/eq?T)作用在![\left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ]](https://latex.csdn.net/eq?%5Cleft%5B%20%5Cbegin%7Barray%7D%7Bc%7D%20x%5C%5C%20y%5C%5C%201%5C%5C%20%5Cend%7Barray%7D%20%5Cright%20%5D)与![\left[ \begin{array}{c} x'\\ y'\\ z'\\ \end{array} \right ]](https://latex.csdn.net/eq?%5Cleft%5B%20%5Cbegin%7Barray%7D%7Bc%7D%20x%27%5C%5C%20y%27%5C%5C%20z%27%5C%5C%20%5Cend%7Barray%7D%20%5Cright%20%5D)。即：

    ![\left[ \begin{array}{c} x'\\ y'\\ z'\\ \end{array} \right ] = T \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ]](https://latex.csdn.net/eq?\left[ \begin{array}{c} x'\\ y'\\ z'\\ \end{array} \right ] %3D T \left[ \begin{array}{c} x\\ y\\ 1\\ \end{array} \right ])

    ```cpp
    //将变换矩阵T应用于原图像，使其透视变换为目标图像。
    void warpPerspective(
    	InputArray src,
    	OutputArray dst,
    	InputArray M,
    	Size dsize,
    	int flags=INTER_LINEAR,
    	int borderMode = BORDER_CONSTANT, 
    	const Scalar& borderValue = Scalar())；
    ```

    src：输入图像。

    dst：输出图像，需要初始化一个空矩阵用来保存结果，不用设定矩阵尺寸。

    M：3x3的转换矩阵。

    dsize：输出图像的大小。

    flags：设置插值方法。默认为INTER_LINEAR表示双线性插值，INTER_NEAREST表示最近邻插值，WARP_INVERSE_MAP表示M作为反转转换 (dst->src) 。

    borderMode：像素外推方法，默认为BORDER_CONSTANT，指定常数填充。翻阅官方文档发现还有一个选项是BORDER_REPLICATE。

    borderValue：常数填充时边界的颜色设置，默认是(0,0,0)，表示黑色。这就是为什么透视变换后图片周围是黑色的原因。这里需要注意的是类型为Scalar (B, G, R)。

    - 映射点标记

      ```cpp
      //在图像上画一个具有给定中心和半径的空心或实心圆。
      circle (
      	InputOutputArray img, 
      	Point center, 
      	int radius, 
      	const Scalar &color, 
      	int thickness=1, 
      	int lineType=LINE_8, 
      	int shift=0)
      ```
      参数img：画圆绘制的图像。
      参数center：画圆的圆心坐标，类型为Scalar(x, y)。
      参数radius：圆的半径。
      参数color：圆的颜色，规则为(B,G,R)，类型为Scalar(B,G,R)。
      参数thickness：如果正数表示组成圆的线条的粗细程度。如果是负数表示圆是否被填充，如FILLED表示要绘制实心的圆。
      参数line_type：线条的类型，默认是LINE_8。
      参数shift：圆心坐标点和半径值的小数点位数，默认为0位。

    $\begin{equation} T^{-1} \left[ \begin{array}{c} x''\\ y''\\ 1\\ \end{array} \right ] = \left[ \begin{array}{c} \frac{x}{z'}\\ \frac{y}{z'}\\ \frac{1}{z'}\\ \end{array} \right ] \end{equation}$	$\begin{equation} \left\{ \begin{matrix}{} x=(c_{11}x''+c_{12}y''+c_{13})z' \\ y=(c_{21}x''+c_{22}y''+c_{23})z'\\ z'=\frac{1}{c_{31}x''+c_{32}y''+c_{33}}\\ \end{matrix} \right. \end{equation}$
    
    ```cpp
    //自己实现的wrapPerspective函数
    void _wrapPerspective(const Mat& src, const Mat& T, Mat& dst)//src为源图像，T为变换矩阵，dst为目标图像
    {
        dst.create(src.size(), src.type());//创建一个和原图像一样大小的Mat
        Mat T_inverse;//变换矩阵的逆
        invert(T, T_inverse);//求矩阵T的逆，结果存到T_inverse
        //取出矩阵中的值
        double c11 = T_inverse.ptr<double>(0)[0];
        double c12 = T_inverse.ptr<double>(0)[1];
        double c13 = T_inverse.ptr<double>(0)[2];
        double c21 = T_inverse.ptr<double>(1)[0];
        double c22 = T_inverse.ptr<double>(1)[1];
        double c23 = T_inverse.ptr<double>(1)[2];
        double c31 = T_inverse.ptr<double>(2)[0];
        double c32 = T_inverse.ptr<double>(2)[1];
        double c33 = T_inverse.ptr<double>(2)[2];
        //遍历目标图像的每个位置，求取原图像对应位置的像素值
        
        for (int y = 0; y < dst.rows; y++)
        {
            for (int x = 0; x < dst.cols; x++)
            {
                double xp = c11 * x + c12 * y + c13;
                double yp = c21 * x + c22 * y + c23;
                double z = c31 * x + c32 * y + c33;//z'
                z = z ? 1.0 / z : 0;//z'不为0时求导数，否则设为0
                xp *= z;
                yp *= z;
                //将双精度坐标限制在整型能表示的最大最小值之间
                double fx = max((double)INT_MIN, min((double)INT_MAX, xp));
                double fy = max((double)INT_MIN, min((double)INT_MAX, yp));
                //转化为int，这里简单地使用了最近邻插值
                int X = saturate_cast<int>(fx);
                int Y = saturate_cast<int>(fy);
                //是否在原图像大小范围内
                if (X >= 0 && X < src.cols && Y >= 0 && Y < src.cols)
                {
                    dst.at<Vec3b>(y, x)[0] = src.at<Vec3b>(Y, X)[0];
                    dst.at<Vec3b>(y, x)[1] = src.at<Vec3b>(Y, X)[1];
                    dst.at<Vec3b>(y, x)[2] = src.at<Vec3b>(Y, X)[2];
                }
                else//以黑色填充
                {
                    dst.at<Vec3b>(y, x)[0] = 0;
                    dst.at<Vec3b>(y, x)[1] = 0;
                    dst.at<Vec3b>(y, x)[2] = 0;
                }
            }
        }
    }
    ```
  
- 透视变换的应用

  - 交互程序

    ```cpp
    //鼠标点击事件
    void setMousecallback(const string& winname, MouseCallback onMouse, void* userdata=0);
    ```

    winname：窗口的名字。
    onMouse：鼠标响应函数或者说回调函数。指定窗口里每次鼠标事件发生的时候，被调用的函数指针。 这个函数的原型为void on_Mouse(int event, int x, int y, int flags, void* param)。
    userdata：传给回调函数的参数，默认为0。这个参数我个人还没用到过。

    ```cpp
    //
    void MouseCallback(int event,int x,int y,int flags,void *useradata);
    ```

    event：鼠标事件。
    x：鼠标事件的x坐标。
    y： 鼠标事件的y坐标。
    flags: 代表鼠标的拖拽事件和键盘鼠标联合的事件。
    userdata : 可选的参数，目前没用到过。
    鼠标事件event主要有下面几种：
            EVENT_MOUSEMOVE ：鼠标移动
            EVENT_LBUTTONDOWN : 鼠标左键按下
            EVENT_RBUTTONDOWN : 鼠标右键按下
            EVENT_MBUTTONDOWN ： 鼠标中键按下
            EVENT_LBUTTONUP ： 鼠标左键放开
            EVENT_RBUTTONUP ： 鼠标右键放开
            EVENT_MBUTTONUP ： 中键放开
            EVENT_LBUTTONDBLCLK ： 左键双击
            EVENT_RBUTTONDBLCLK ： 右键双击
            EVENT_MBUTTONDBLCLK ： 中键双击
    Flags主要有一下几种：
            EVENT_FLAG_LBUTTON ：左键拖拽
            EVENT_FLAG_RBUTTON ： 右键拖拽
            EVENT_FLAG_MBUTTON ： 中键拖拽
            EVENT_FLAG_CTRLKEY ： Ctrl按下不放
            EVENT_FLAG_SHIFTKEY ： shift按下不放
            EVENT_FLAG_ALTKEY ： alt按下不放











\left[\matrix{}\right]
$$

$$

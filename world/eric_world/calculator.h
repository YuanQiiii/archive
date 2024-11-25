#ifndef CALCULATOR_H
#define CALCULATOR_H
#endif // CALCULATOR_H
class Point {  // 使用四元数表示三维坐标
public:
    double position[4]{0};
    Point() {
        for(int i = 0;i<4;i++){
            position[i] = 0;
        }
    }
    Point(double _x, double _y, double _z, double _w) { // 初始化
        position[0] = _x;
        position[1] = _y;
        position[2] = _z;
        position[3] = _w;
    }
};

class Matrix { // 矩阵及其计算
public:
    double matrix[4][4]{0};
    Matrix multiplication_right(const Matrix& right) ; // 右乘
    Matrix multiplication_left(const Matrix& left); // 左乘
    Matrix addition(const Matrix& right); // 加法
    Point transformation_right(const Point& origin) const; // 右乘向量
    Matrix(bool flag = false) { // 初始化
        // 生成0矩阵
        for(int i = 0;i<4;i++){
            for(int j = 0;j<4;j++){
                matrix[i][j] = 0;
            }
        }
        // 生成单位矩阵
        if(flag){
            matrix[0][0]= 1;
            matrix[1][1]= 1;
            matrix[2][2]= 1;
            matrix[3][3]= 1;
        }
    }
};

class Transformation { // 变换
public:
    Matrix translation, rotation, projection, zoomer,combination; // 平移,旋转,投影,缩放,变换组合
    Transformation() {
        translation = Matrix(true);
        rotation = Matrix(true);
        projection = Matrix(true);
        zoomer = Matrix(true);
        combination = Matrix(true);
    }
    // 平移到以origin为原点
    void translate(const Point& origin);
    // 按照指定旋转轴旋转
    // x{1,0,0,0},y{0,1,0,0},z{0,0,1,0}
    void rotate(double angle, const Point& axis);
    /*
投影矩阵参数的作用

视场角 (Field of View, fov):
视场角表示相机视野的宽度范围，可以理解为从相机位置看到的场景的“宽度”。
视场角通常以弧度表示，可以通过 fov = degrees * (PI / 180.0) 将角度转换为弧度。
较大的视场角会使图像显得更广阔，但同时也可能引入更多的透视失真；较小的视场角则会使图像显得更集中。

纵横比 (Aspect Ratio):
纵横比是图像宽度与高度的比值，用于保持图像不变形。
通常计算方式为 aspect = width / height。
不同的显示设备（如宽屏、普通屏幕）有不同的纵横比，这一参数能确保图像在不同设备上都能正确显示。

近平面距离 (Near Plane Distance):
近平面是从相机开始的一个距离，任何在这个距离之前的物体都不会被渲染。
设定合适的近平面距离可以避免在极近距离的渲染问题，例如去除相机自身的渲染。
该值通常设置为一个较小的正数。

远平面距离 (Far Plane Distance):
远平面是从相机开始的一段距离，任何在这个距离之后的物体都不会被渲染。
设定合适的远平面距离可以限制渲染的场景深度，减少计算开销。
该值通常设置为一个较大的正数。
这些参数共同作用，确定了相机的视锥体（view frustum），即从相机位置到远平面之间的可见空间范围。
*/
    void project(double fov, double aspect, double near, double far, double scale);
    // 缩放,参数为x,y,z的缩放倍数
    void zoom(double x,double y,double z);
    // 组合变换矩阵(逐个左乘)
    Matrix combine();
};

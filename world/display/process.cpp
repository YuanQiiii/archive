#include "process.h"
#include<cmath>
#include <cstdlib>
#include <ctime>
Point myorigin = {2, 3, 5};
Angle myangle = {0, 0, 90};
QString showdata(){
    QString ans;
    ans += "position:("+std::to_string(myorigin.x)+","+std::to_string(myorigin.y)+","+std::to_string(myorigin.z)+")";
    ans+= " angle:("+std::to_string(myangle.x)+","+std::to_string(myangle.y)+","+std::to_string(myangle.z)+")";
    return ans;
}

void center(Point &p, const Point &origin)
{
    p.x -= origin.x;
    p.y -= origin.y;
    p.z -= origin.z;
}

void rotate(Point &p, const Angle &angle)
{
    double x, y, z;

    // Convert angles to radians
    double rad_x = angle.x * M_PI / 180.0;
    double rad_y = angle.y * M_PI / 180.0;
    double rad_z = angle.z * M_PI / 180.0;

    // Rotate around z-axis
    x = p.x;
    y = p.y;
    p.x = x * cos(rad_z) - y * sin(rad_z);
    p.y = x * sin(rad_z) + y * cos(rad_z);

    // Rotate around y-axis
    x = p.x;
    z = p.z;
    p.x = x * cos(rad_y) + z * sin(rad_y);
    p.z = -x * sin(rad_y) + z * cos(rad_y);

    // Rotate around x-axis
    y = p.y;
    z = p.z;
    p.y = y * cos(rad_x) - z * sin(rad_x);
    p.z = y * sin(rad_x) + z * cos(rad_x);
}

Point projection(const Point &p, const int focus)
{
    Point result = p;
    if (result.z != 0)
    {
        result.x = (result.x * focus) / result.z;
        result.y = (result.y * focus) / result.z;
    }
    result.z = 0;
    return result;
}

std::vector<Point> match(const std::pair<Point, Point> &line, const int number)
{
    std::vector<Point> points_on_line;
    double x_d = line.second.x - line.first.x;
    double y_d = line.second.y - line.first.y;
    double z_d = line.second.z - line.first.z;
    double alpha = 0;
    for (int i = 0; i <= number; i++)
    {
        points_on_line.push_back({line.first.x + x_d * alpha, line.first.y + y_d * alpha, line.first.z + z_d * alpha});
        alpha += 1.0 / number;
    }
    return points_on_line;
}

Point generateRandomPointOnSphere(double _x,double _y,double _z,double _r) {
    double theta = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI;
    double phi = static_cast<double>(rand()) / RAND_MAX * M_PI;

    double x = _r*sin(phi) * cos(theta) + _x;
    double y = _r*sin(phi) * sin(theta)+ _y;
    double z = _r*cos(phi)+ _z;

    return {x, y, z};
}

std::vector<Point> createball(double x,double y,double z,double r){
    srand(static_cast<unsigned int>(time(nullptr))); // 初始化随机数生成器

    const int numPoints = 50000; // 生成点的数量
    std::vector<Point> points;

    for (int i = 0; i < numPoints; ++i) {
        Point point = generateRandomPointOnSphere(x,y,z,r);
        points.push_back(point);
    }
    return points;
}

#include"creator.h"
#include<cmath>
#include <cstdlib>
#include <ctime>
#define M_PI 3.14159265358979323846
std::vector<Point> match(const std::pair<Point, Point> &line, const int number)
{
    std::vector<Point> points_on_line;
    double x_d = line.second.position[0] - line.first.position[0];
    double y_d = line.second.position[1] - line.first.position[1];
    double z_d =  line.second.position[2] - line.first.position[2];
    double alpha = 0;
    for (int i = 0; i <= number; i++)
    {
        points_on_line.push_back({line.first.position[0] + x_d * alpha, line.first.position[1] + y_d * alpha, line.first.position[2] + z_d * alpha,1});
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

    return {x, y, z,1};
}

std::vector<Point> createball(double x,double y,double z,double r){
    srand(static_cast<unsigned int>(time(nullptr))); // 初始化随机数生成器

    const int numPoints = 5000; // 生成点的数量
    std::vector<Point> points;

    for (int i = 0; i < numPoints; ++i) {
        Point point = generateRandomPointOnSphere(x,y,z,r);
        points.push_back(point);
    }
    return points;
}
std::vector<Point> createcube(double x,double y,double z,double l){
    std::vector<std::pair<Point, Point>> lines{
        {{1, 1, 1,1}, {1, 1, -1,1}},
        {{1, 1, 1,1}, {1, -1, 1,1}},
        {{1, 1, 1,1}, {-1, 1, 1,1}},

        {{-1, -1, -1,1}, {-1, -1, 1,1}},
        {{-1, -1, -1,1}, {-1, 1, -1,1}},
        {{-1, -1, -1,1}, {1, -1, -1,1}},

        {{1, -1, 1,1}, {1, -1, -1,1}},
        {{1, -1, 1,1}, {-1, -1, 1,1}},
        {{-1, 1, 1,1}, {-1, -1, 1,1}},

        {{-1, 1, 1,1}, {-1, 1, -1,1}},
        {{1, 1, -1,1}, {1, -1, -1,1}},
        {{1, 1, -1,1}, {-1, 1, -1,1}},


        {{10, 0, 0,1}, {-10, 0, 0,1}},
        {{0, 10, 0,1}, {0, -10, 0,1}},
        {{0, 0, -10,1}, {0, 0, 10,1}}
    };
    std::vector<Point> points;
    for (auto &line : lines)
    {
        std::vector<Point> points_on_line = match(line, 5000);
        for (auto &point : points_on_line)
        {
            points.push_back(point);
        }
    }
    return points;
}
std::vector<Point> globalcreator(){
     std::vector<Point> points;
    std::vector<Point> ball1 = createball(2,2,2,1);
    for (auto &point: ball1)
    {
        points.push_back(point);
    }
    std::vector<Point> ball2 = createball(0,0,0,0.3);
    for (auto &point: ball2)
    {
        points.push_back(point);
    }
    std::vector<Point> cube1 = createcube(1,1,1,2);
    for (auto &point: cube1)
    {
        points.push_back(point);
    }


    // std::vector<Point> heart1 = createHeart(1.5);
    // for (auto &point: heart1)
    // {
    //     points.push_back(point);
    // }


    // double x = 0.0, y = 0.0, z = 0.0, sideLength = 10.0; // 四面体的起始坐标和边长
    // std::vector<Point> tetrahedron1= createTetrahedron( x,  y, z, sideLength);
    // for (auto &point: tetrahedron1)
    // {
    //     points.push_back(point);
    // }
    return points;
}

Point generateRandomPointOnHeart(double scale) {
    double theta = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI;
    double t = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI;

    double r = scale * (2 - 2 * sin(theta) + sin(theta) * sqrt(fabs(cos(theta))) / (sin(theta) + 1.4));
    double x = r * cos(theta) * sin(t);
    double y = r * cos(theta) * cos(t)*0.5;
    double z = r * sin(theta);

    return {x, y, z, 1};
}

std::vector<Point> createHeart(double scale) {
    srand(static_cast<unsigned int>(time(nullptr))); // 初始化随机数生成器

    const int numPoints = 5000; // 生成点的数量
    std::vector<Point> points;

    for (int i = 0; i < numPoints; ++i) {
        Point point = generateRandomPointOnHeart(scale);
        points.push_back(point);
    }
    return points;
}

Point interpolate(const Point& p1, const Point& p2, double t) {
    return {
        p1.position[0] * (1 - t) + p2.position[0]* t,
        p1.position[1] * (1 - t) + p2.position[1] * t,
        p1.position[2] * (1 - t) + p2.position[2] * t,
        1
    };
}

Point generateRandomPointInTetrahedron(const std::vector<Point>& vertices) {
    double t1 = static_cast<double>(rand()) / RAND_MAX;
    double t2 = static_cast<double>(rand()) / RAND_MAX;
    double t3 = static_cast<double>(rand()) / RAND_MAX;

    // Ensure the points lie within the tetrahedron
    if (t1 + t2 > 1) {
        t1 = 1 - t1;
        t2 = 1 - t2;
    }
    if (t1 + t2 + t3 > 1) {
        double sum = t1 + t2 + t3;
        t1 = t1 / sum;
        t2 = t2 / sum;
        t3 = t3 / sum;
    }

    Point p = interpolate(vertices[0], vertices[1], t1);
    Point q = interpolate(vertices[0], vertices[2], t2);
    Point r = interpolate(p, q, t3);

    return r;
}

std::vector<Point> createTetrahedron(double x, double y, double z, double sideLength) {
    srand(static_cast<unsigned int>(time(nullptr))); // 初始化随机数生成器

    const int numPoints = 5000; // 生成点的数量
    std::vector<Point> points;

    // Define the vertices of the tetrahedron
    double height = std::sqrt(2.0 / 3.0) * sideLength;
    std::vector<Point> vertices = {
        {x, y, z, 1},
        {x + sideLength, y, z, 1},
        {x + sideLength / 2, y + std::sqrt(3.0) * sideLength / 2, z, 1},
        {x + sideLength / 2, y + std::sqrt(3.0) * sideLength / 6, z + height, 1}
    };

    for (int i = 0; i < numPoints; ++i) {
        Point point = generateRandomPointInTetrahedron(vertices);
        points.push_back(point);
    }

    return points;
}

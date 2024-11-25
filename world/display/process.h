#pragma once
#ifndef PROCESS_H
#define PROCESS_H
#include<vector>
#include<Qstring>
#endif // PROCESS_H
struct Angle
{
    double x, y, z; // 绕x,y,z轴的旋转角度
};

struct Point
{
    double x, y, z;
};
extern Point myorigin;
extern Angle myangle;
void center(Point &p, const Point &origin = myorigin);
void rotate(Point &p, const Angle &angle = myangle);
Point projection(const Point &p, const int focus);
std::vector<Point> match(const std::pair<Point, Point> &line, const int number);
QString showdata();
Point generateRandomPointOnSphere(double _x,double _y,double _z,double _r);
std::vector<Point> createball(double x,double y,double z,double r);

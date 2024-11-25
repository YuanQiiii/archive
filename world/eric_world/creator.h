#ifndef CREATOR_H
#define CREATOR_H
#include<vector>
#include"calculator.h"
#endif // CREATOR_H
std::vector<Point> match(const std::pair<Point, Point> &line, const int number);
Point generateRandomPointOnSphere(double _x,double _y,double _z,double _r);
std::vector<Point> createball(double x,double y,double z,double r);
std::vector<Point> createcube(double x,double y,double z,double l);
std::vector<Point> globalcreator();
Point generateRandomPointOnHeart(double scale);
std::vector<Point> createHeart(double scale) ;
std::vector<Point> createTetrahedron(double x, double y, double z, double sideLength);

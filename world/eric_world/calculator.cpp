#include "calculator.h"
#include<cmath>
Matrix Matrix::multiplication_right(const Matrix& right)  {
    Matrix ans;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                ans.matrix[i][j] += this->matrix[i][k] * right.matrix[k][j];
            }
        }
    }
    return ans;
}

Matrix Matrix::multiplication_left(const Matrix& left) {
    Matrix ans;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                ans.matrix[i][j] += left.matrix[i][k] * this->matrix[k][j];
            }
        }
    }
    return ans;
}

Matrix Matrix::addition(const Matrix& right) {
    Matrix ans;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ans.matrix[i][j] = this->matrix[i][j] + right.matrix[i][j];
        }
    }
    return ans;
}

Point Matrix::transformation_right(const Point& origin) const {
    Point ans;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ans.position[i] += this->matrix[i][j] * origin.position[j];
        }
    }
    return ans;
}

void  Transformation::rotate(double angle, const Point& axis) { // 修复旋转矩阵不可叠加的问题
    Matrix origin = rotation;
    double c = cos(angle);
    double s = sin(angle);
    double t = 1 - c;
    rotation.matrix[0][0] = t * axis.position[0] * axis.position[0] + c;
    rotation.matrix[0][1] = t * axis.position[0] * axis.position[1] - s * axis.position[2];
    rotation.matrix[0][2] = t * axis.position[0] * axis.position[2] + s * axis.position[1];
    rotation.matrix[1][0] = t * axis.position[0] * axis.position[1] + s * axis.position[2];
    rotation.matrix[1][1] = t * axis.position[1] * axis.position[1] + c;
    rotation.matrix[1][2] = t * axis.position[1] * axis.position[2] - s * axis.position[0];
    rotation.matrix[2][0] = t * axis.position[0] * axis.position[2] - s * axis.position[1];
    rotation.matrix[2][1] = t * axis.position[1] * axis.position[2] + s * axis.position[0];
    rotation.matrix[2][2] = t * axis.position[2] * axis.position[2] + c;
    rotation.matrix[3][3] = 1; // 添加齐次坐标
    rotation = rotation.multiplication_right(origin);
}
void Transformation::project(double fov, double aspect, double near, double far, double scale) {
    // Initialize to identity matrix
    projection = Matrix(true);

    double tanHalfFovy = tan(fov / 2.0);
    projection.matrix[0][0] = scale / (aspect * tanHalfFovy);
    projection.matrix[1][1] = scale / tanHalfFovy;
    projection.matrix[2][2] = -(far + near) / (far - near);
    projection.matrix[2][3] = -1.0;
    projection.matrix[3][2] = -(2.0 * far * near) / (far - near);
    projection.matrix[3][3] = 0.0; // 确保齐次坐标w正确设置
}

void Transformation:: translate(const Point& origin) {
    translation.matrix[0][3] = -origin.position[0];
    translation.matrix[1][3] =-origin.position[1];
    translation.matrix[2][3] =-origin.position[2];
    translation.matrix[3][3] = 1; // 添加齐次坐标
}
void Transformation::zoom(double alpha,double bata,double gamma){
    zoomer.matrix[0][0]= alpha;
    zoomer.matrix[1][1]= bata;
    zoomer.matrix[2][2]= gamma;
    zoomer.matrix[3][3]= 1;
}
Matrix Transformation:: combine(){

    Matrix ans = this->translation;
    ans = ans.multiplication_left(this->rotation);
    ans = ans.multiplication_left(this->zoomer);
    ans = ans.multiplication_left(this->projection);
    this->combination = ans;
    return combination;
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <thread>
#include<QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), frameCount(0), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frameTimer = new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, &MainWindow::updateFrameRate);
    frameTimer->start(1000); // 每秒更新一次
}

MainWindow::~MainWindow()
{
    delete ui;
}

void transformPoints(const Matrix& transformation_matrix, const std::vector<Point>& points, std::vector<Point>& todraw, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        todraw[i] = transformation_matrix.transformation_right(points[i]);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    frameCount++; // 每帧增加计数

    std::vector<Point> points = globalcreator();

    Transformation transformation;
    transformation.translate(myposition);
    transformation.zoom(1, 1, 1);
    transformation.rotate(lr * M_PI / 180.0, Point(1, 0, 0, 0)); // Horizontal rotation
    transformation.rotate(ud * M_PI / 180.0, Point(0, 0, 1, 0)); // Vertical rotation
    transformation.rotate(ud * M_PI / 180.0, Point(0, 1, 0, 0)); // Vertical rotation
    transformation.project(100 , 1, 0.01, 1000, 30.0); // 添加缩放因子

    Matrix transformation_matrix = transformation.combine();
    std::vector<Point> todraw(points.size());

    size_t num_threads = std::thread::hardware_concurrency(); // 获取硬件线程数
      qDebug() << "Number of threads used:" << num_threads; // 输出线程数
    size_t chunk_size = points.size() / num_threads;

    std::vector<std::thread> threads;

    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = (t == num_threads - 1) ? points.size() : start + chunk_size; // 最后一个线程处理剩余的点
        threads.emplace_back(transformPoints, std::cref(transformation_matrix), std::cref(points), std::ref(todraw), start, end);
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    pcount = int(todraw.size());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), Qt::black);

    // 绘制每个点
    for (const Point &p : todraw) {
        // 随机生成颜色
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        QPen pen(QColor(r, g, b));
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawPoint(QPointF(p.position[0] + width() / 2, p.position[1] + height() / 2));
    }
}

// 其他事件处理函数...



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    const double moveSpeed = 0.5;

    switch (event->key())
    {
    case Qt::Key_W:
        myposition.position[0] += moveSpeed;
        break;
    case Qt::Key_S:
        myposition.position[0] -=  moveSpeed;
        break;
    case Qt::Key_A:

        myposition.position[1] -=  moveSpeed;
        break;
    case Qt::Key_D:

        myposition.position[1] +=  moveSpeed;
        break;
    case Qt::Key_Q:
        myposition.position[2] += moveSpeed;
        break;
    case Qt::Key_E:
        myposition.position[2] -= moveSpeed;
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
    update();
     ui->datalabel->setText(showdata());
    qDebug() << "keyboard";
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->pos();
    if (firstMouseMove) {
        lastMousePos = currentPos;
        firstMouseMove = false;
    }

    int dx = currentPos.x() - lastMousePos.x();
    int dy = currentPos.y() - lastMousePos.y();

    lr -= dx * 0.1; // 水平旋转 (绕 Y 轴)
    ud += dy * 0.1; // 垂直旋转 (绕 X 轴)

    // 限制俯仰角度以避免翻转
    if (ud > 89.0)
        ud = 89.0;
    if (ud < -89.0)
        ud = -89.0;

    lastMousePos = currentPos;

    update();
    qDebug() << "mouse";
    ui->datalabel->setText(showdata());
}



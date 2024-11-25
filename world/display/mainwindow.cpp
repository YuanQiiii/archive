#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    std::vector<std::pair<Point, Point>> lines{
                                               {{1, 1, 1}, {1, 1, -1}},
                                               {{1, 1, 1}, {1, -1, 1}},
                                               {{1, 1, 1}, {-1, 1, 1}},
                                               {{-1, -1, -1}, {-1, -1, 1}},
                                               {{-1, -1, -1}, {-1, 1, -1}},
                                               {{-1, -1, -1}, {1, -1, -1}},
                                               {{1, -1, 1}, {1, -1, -1}},
                                               {{1, -1, 1}, {-1, -1, 1}},
                                               {{-1, 1, 1}, {-1, -1, 1}},
                                               {{-1, 1, 1}, {-1, 1, -1}},
                                               {{1, 1, -1}, {1, -1, -1}},
                                               {{1, 1, -1}, {-1, 1, -1}},
        {{1,0,100},{1,0,-100}},{{3,0,100},{3,0,-100}},
        {{1000,0,0},{-1000,0,0}},{{0,1000,0},{0,-1000,0}},{{0,0,1000},{0,0,-1000}}
                                               };

    std::vector<Point> points;
    for (auto &line : lines)
    {
        std::vector<Point> points_on_line = match(line, 5000);
        for (auto &point : points_on_line)
        {
            center(point);
            rotate(point);
            point = projection(point, 100);
            points.push_back(point);
        }
    }
    std::vector<Point> ball = createball(2,2,2,1);
    for (auto &point: ball)
    {            center(point);
        rotate(point);
        point = projection(point, 100);
        points.push_back(point);}
    std::vector<Point> ball2 = createball(0,0,0,0.3);
    for (auto &point: ball2)
    {            center(point);
        rotate(point);
        point = projection(point, 100);
        points.push_back(point);}

    std::cout << "Number of points: " << points.size() << std::endl;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 设置画笔颜色和大小
    QPen pen(Qt::white);
    pen.setWidth(1);
    painter.setPen(pen);

    // 设置背景颜色，以确保与点的颜色形成对比
    painter.fillRect(this->rect(), Qt::black);

    // 绘制每个点
    for (const Point &p : points)
    {
        // 将点的坐标平移到窗口中心
        painter.drawPoint(QPointF(p.x + width() / 2, p.y + height() / 2));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 定义前后左右移动的速度
    const double moveSpeed = 0.5;

    // 计算方向向量，基于当前的角度
    double radY = myangle.y * M_PI / 180.0;
    double forwardX = cos(radY);
    double forwardZ = sin(radY);

    double rightX = forwardZ;
    double rightZ = -forwardX;

    switch (event->key())
    {
    case Qt::Key_W:
        myorigin.x += forwardX * moveSpeed;
        myorigin.z += forwardZ * moveSpeed;
        break;
    case Qt::Key_S:
        myorigin.x -= forwardX * moveSpeed;
        myorigin.z -= forwardZ * moveSpeed;
        break;
    case Qt::Key_A:
        myorigin.x -= rightX * moveSpeed;
        myorigin.z -= rightZ * moveSpeed;
        break;
    case Qt::Key_D:
        myorigin.x += rightX * moveSpeed;
        myorigin.z += rightZ * moveSpeed;
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
    update(); // 刷新画面
    ui->datalabel->setText(showdata());
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->pos();
    int dx = currentPos.x() - lastMousePos.x();
    int dy = currentPos.y() - lastMousePos.y();

    myangle.z += dx * 0.05; // 根据鼠标移动调整角度

    myangle.y += dy * 0.05;

    lastMousePos = currentPos;

    update(); // 刷新画面
    ui->datalabel->setText(showdata());
}

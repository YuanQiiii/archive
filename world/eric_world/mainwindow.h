#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include"creator.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int frameCount;         // 帧计数
    QTimer* frameTimer;     // 定时器
    void updateFrameRate() {
        qDebug() << "Current Frame Rate:" << frameCount;
        frameCount = 0; // 重置帧计数
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Point myposition{2,2,2,0};
    double lr = 0,ud = 0;
    int pcount = 0;
    QString showdata(){
        qDebug()<<"work";
        QString ans;
        ans += "position :("+std::to_string(myposition.position[0])+","+std::to_string(myposition.position[1])+","+std::to_string(myposition.position[2])+")";
        ans+= "\n";
        ans += "left-right:"+std::to_string(lr);
         ans+= "\n";
        ans += "up-down:"+std::to_string(ud);
          ans+= "\n";
        ans += "points:"+std::to_string(pcount);
        return ans;
    }


private:
    Ui::MainWindow *ui;
    QPoint lastMousePos;
    bool firstMouseMove = true; // 标记第一次鼠标移动
};
#endif // MAINWINDOW_H

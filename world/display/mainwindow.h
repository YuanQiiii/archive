#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include "process.h"
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QPoint lastMousePos;
    QString showdata() {
        return QString("Origin: (%1, %2, %3)\nAngle: (%4, %5, %6)")
            .arg(myorigin.x)
            .arg(myorigin.y)
            .arg(myorigin.z)
            .arg(myangle.x)
            .arg(myangle.y)
            .arg(myangle.z);
    }
};

#endif // MAINWINDOW_H

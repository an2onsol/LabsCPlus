#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPixmap>
#include <QWidget>
#include "render.h"
#include <QPainter>
#include <QKeyEvent>

class Fractal : public QWidget
{
    Q_OBJECT

public:
    Fractal(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void updatePixelMap(const QImage &image, double scaleFactor);
    void zoom(double zoomFactor);

private:
    const double iZoom = 0.5;
    const double oZoom = 2;
    const int ScrollStep = 20;

    void scroll(int deltaX, int deltaY);

    Render thread;
    QPoint pixmapOffset;
    QPoint lastPosition;
    QPixmap pixmap;

    double cX;
    double cY;
    double dscale;
    double cscale;
};
#endif // MAINWINDOW_H

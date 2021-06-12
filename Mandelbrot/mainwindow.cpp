#include "mainwindow.h"
#include "ui_mainwindow.h"

Fractal::Fractal(QWidget *parent) : QWidget(parent) {
    setWindowTitle("FractalMandelbrot");

    cX = -0.6;
    cY = -0.2;
    dscale = 0.005;
    cscale = 0.005;

    connect(&thread, &Render::renderedImage, this, &Fractal::updatePixelMap);
    setCursor(Qt::CrossCursor);
    resize(1920, 1080);
}

void Fractal::paintEvent(QPaintEvent * ) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (qFuzzyCompare(cscale, dscale)) {
        painter.drawPixmap(pixmapOffset, pixmap);
    }
    else {

        auto previewPixmap = qFuzzyCompare(pixmap.devicePixelRatioF(), qreal(1)) ? pixmap : pixmap.scaled(pixmap.size() / pixmap.devicePixelRatioF(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        double scale = dscale / cscale;

        int width = int(previewPixmap.width() * scale);
        int height = int(previewPixmap.height() * scale);

        painter.save();
        painter.translate(pixmapOffset.x() + (previewPixmap.width() - width) / 2, pixmapOffset.y() + (previewPixmap.height() - height) / 2);
        painter.scale(scale, scale);

        QRectF exposed = painter.transform().inverted().mapRect(rect()).adjusted(-1, -1, 1, 1);
        painter.drawPixmap(exposed, previewPixmap, exposed);
        painter.restore();
    }
}

void Fractal::resizeEvent(QResizeEvent * ) {
    thread.render(cX, cY, cscale, size(), devicePixelRatioF());
}

void Fractal::zoom(double szoom) {
    cscale *= szoom;
    repaint();
    thread.render(cX, cY, cscale, size(), devicePixelRatioF());
}

void Fractal::scroll(int scrollx, int scrolly) {
    cX += scrollx * cscale;
    cY += scrolly * cscale;
    repaint();
    thread.render(cX, cY, cscale, size(), devicePixelRatioF());
}

void Fractal::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Plus:
        zoom(iZoom);
        break;
    case Qt::Key_Minus:
        zoom(oZoom);
        break;
    case Qt::Key_Down:
        scroll(0, -ScrollStep);
        break;
    case Qt::Key_Up:
        scroll(0, +ScrollStep);
        break;
    case Qt::Key_Left:
        scroll(-ScrollStep, 0);
        break;
    case Qt::Key_Right:
        scroll(+ScrollStep, 0);
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void Fractal::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        lastPosition = event->pos();
}

void Fractal::wheelEvent(QWheelEvent *event) {
    double steps = (event->angleDelta().y() / 8) / double(15);
    zoom(pow(iZoom, steps));
}

void Fractal::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        pixmapOffset += event->pos() - lastPosition;
        lastPosition = event->pos();
        repaint();
    }
}

void Fractal::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        pixmapOffset += event->pos() - lastPosition;
        lastPosition = QPoint();
        const auto pixmapSize = pixmap.size() / pixmap.devicePixelRatioF();
        int deltaX = (width() - pixmapSize.width()) / 2 - pixmapOffset.x();
        int deltaY = (height() - pixmapSize.height()) / 2 - pixmapOffset.y();
        scroll(deltaX, deltaY);
    }
}

void Fractal::updatePixelMap(const QImage &image, double scale) {
    if (!lastPosition.isNull()){
        return;
    }
    pixmap = QPixmap::fromImage(image);
    pixmapOffset = QPoint();
    lastPosition = QPoint();
    dscale = scale;
    repaint();
}




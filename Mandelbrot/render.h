#ifndef RENDER_H
#define RENDER_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QImage>
#include <QWaitCondition>

class Render : public QThread
{
    Q_OBJECT

public:
    Render(QObject *parent = nullptr);
    ~Render();
    void render(double cX, double cY, double dscale, QSize rsize, double coeff);

signals:
    void renderedImage(const QImage &image, double scaleFactor);

protected:
    void run() override;

private:
    static uint lengthOfWave(double wave);

    QMutex mutex;
    QWaitCondition condition;

    double cX;
    double cY;
    double dscale;
    double coeff;
    QSize rsize;

    bool interrapt = false;
    bool restart = false;

    enum {
        ColormapSize = 512
    };

    uint colormap[ColormapSize];
};


#endif // RENDER_H

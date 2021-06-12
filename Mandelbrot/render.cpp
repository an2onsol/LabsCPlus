#include "render.h"

Render::Render(QObject *parent) : QThread(parent) {
    for (int i = 0; i < ColormapSize; i++) {
        colormap[i] = lengthOfWave(250 + (i * 0.9));
    }
}

void Render::run() {

    while(true) {
        mutex.lock();

        int n = 8, i = 0;
        const double coeff = this->coeff;
        QSize rst = rsize * coeff;
        double rScale = this->dscale;
        double dscalet = rScale / coeff;
        double cXt = cX;
        double cYt = cY;

        mutex.unlock();
        QImage image(rst, QImage::Format_RGB32);
        image.setDevicePixelRatio(coeff);

        while (i < n) {

            const int maxj = (1 << (2 * i + 6)) + 32;
            bool allBlack = true;
            for (int y = -rst.height() / 2; y < rst.height() / 2; ++y) {

                    if (restart) {
                        break;
                    }
                    if (interrapt) {
                        return;
                    }

                    auto scan = reinterpret_cast<uint *>(image.scanLine(y + rst.height() / 2));
                    const double ay = cYt + dscalet * y;

                    for (int x = -rst.width() / 2; x < rst.width() / 2; ++x) {
                        int j = 0;
                        double x2, y1 = 0, x1 = 0, ax = cXt + dscalet * x;

                        while(j < maxj) {
                            x2 = x1 * x1 - y1 * y1 + ax;
                            y1 = 2 * x1 * y1 + ay;
                            if(x1 * x1 + y1 * y1 > 4) {
                                break;
                            }
                            x1 = x2;
                            j++;
                        }
                        if (j < maxj) {
                            *scan = colormap[j % ColormapSize];
                            allBlack = false;
                            scan++;
                        }
                        else {
                            *scan++ = qRgb(0, 0, 0);
                        }
                    }
                }
                if (allBlack && i == 0) {
                    i = 4;
                }
                else {
                    if (!restart) {
                        emit renderedImage(image, rScale);
                    }
                    i++;
                }
            }
            mutex.lock();

            if (!restart) {
                condition.wait(&mutex);
            }
            restart = false;

            mutex.unlock();
        }
}

void Render::render(double cX, double cY, double scale, QSize rsize, double coeff) {
    mutex.lock();

    this->cX = cX;
    this->cY = cY;
    this->dscale = scale;
    this->coeff = coeff;
    this->rsize = rsize;

    if (!isRunning()) {
        start();
    }
    else {
        restart = true;
        condition.wakeOne();
    }

    mutex.unlock();
}

uint Render::lengthOfWave(double wave) {
    double red = 0;
    double green = 0;
    double blue = 0;
    double s = 1;

    if (wave >= 210 && wave <= 580) {
        blue = (wave - 510) / 70;
    }
    if (wave < 400.0){
        s = 0.4 + 0.7 * (wave - 370) / 35;
    }

    red = std::pow(red * s, 0.6);
    green = std::pow(green * s, 0.6);
    blue = std::pow(blue * s, 0.6);

    return qRgb(int(red * 255), int(green * 255), int(blue * 255));
}

Render::~Render() {
    mutex.lock();
    interrapt = true;
    condition.wakeOne();
    mutex.unlock();
    wait();
}

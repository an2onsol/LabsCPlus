#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QtDebug>
#include <QString>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    parseJson();
    fillStates();
    resize(1920, 1080);
}

void MainWindow::parseJson() {

    QFile file("coords.json");
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(data));
    QJsonObject jsonObject= jsonDocument.object();
    QStringList jkeys = jsonObject.keys();
    for (int i = 0; i < jkeys.size(); i++) {
        states.push_back(State());
    }
    QPolygonF polygon;
    int n = 0;
    for (const auto &key : jkeys) {
        for (const auto &i : jsonObject[key].toArray()) {
            polygon.clear();
            QJsonArray c1 = i.toArray().size() > 1 ? i.toArray() : i.toArray().first().toArray();
            for (const auto &j : c1) {
                QPointF c2 = QPointF(j.toArray().first().toDouble() * 10 + 2150, 900 - 10 * j.toArray().last().toDouble());
                polygon << c2;
            }
            states[n].polygons.push_back(polygon);
        }
        n++;
    }
    file.close();
}

void MainWindow::fillStates() {

    QMap<QString, double> sentiments;
    QString tempString;
    QStringList tempList;
    QFile file("sentiments.csv");
    file.open(QIODevice::ReadOnly);
    QTextStream data(&file);
    while(!data.atEnd()){
        tempString = data.readLine();
        tempList = tempString.split(',');
        sentiments.insert(tempList[0], tempList[1].toDouble());
    }
    file.close();

    QFile fileT("tweets.txt");
    fileT.open(QIODevice::ReadOnly);
    QTextStream dataT(&fileT);
    QRegularExpression tweet("(\\d{2})\\t(.+)");
    QRegularExpression rpoint("(\\-?\\d+\\.{1}\\d+).{2}(\\-?\\d+\\.{1}\\d+)");

    while(!dataT.atEnd()) {
        tempString = dataT.readLine();
        QRegularExpressionMatch pointm = rpoint.match(tempString);
        QRegularExpressionMatch tweetm = tweet.match(tempString);

        QList wArray = tweetm.captured().split("\t").last().split(" ");
        QList cArray = pointm.captured().split(", ");

        if(pointm.hasMatch()) {
            QPointF point = QPointF(10 * cArray.last().toDouble() + 2150, 900 - cArray.first().toDouble() * 10);
            for(auto &state : states) {
                for(const auto &polygon : state.polygons) {
                    if(polygon.containsPoint(point, Qt::OddEvenFill)) {
                        for(const auto &tweet : wArray){
                            if(sentiments.find(tweet) != sentiments.end()){
                                state.status += sentiments.find(tweet).value();
                            }
                        }
                    }
                }
            }
        }
    }
    fileT.close();
}
void MainWindow::paintEvent(QPaintEvent*) {
    QPainter *qpainter;
    QPen qpen(Qt::black, 3);
    qpainter = new QPainter(this);
    qpainter->setPen(qpen);
    for (const auto &state : states) {
        if (state.status){
            if (state.status > 255){
                qpainter->setBrush(QColor(qRgb(255, 0, 0)));
            }
            else {
                qpainter->setBrush(QColor(qRgb(255, 255 - state.status, 255 - state.status)));
            }
        }
        else {
            if (state.status < -255){
                qpainter->setBrush(QColor(qRgb(0, 0, 255)));
            }
            else {
                qpainter->setBrush(QColor(qRgb(255 + state.status, 255 + state.status, 255)));
            }
        }
        for(const auto &polygon : state.polygons) {
            qpainter->drawPolygon(polygon);
        }
    }
    delete qpainter;
}

MainWindow::~MainWindow() {
    delete ui;
}


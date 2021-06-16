#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void parseJson();
    void fillStates();
    struct State {
        double status = 0;
        QList<QPolygonF> polygons;
    };
    void paintEvent(QPaintEvent*);
    QList<State> states;

};
#endif // MAINWINDOW_H

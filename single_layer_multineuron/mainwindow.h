#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QColor>
#include <math.h>

static QColor colorArray [5]  = {Qt::red  , Qt::green , Qt::yellow , Qt::blue  , Qt::gray};

struct MyPoint{

  MyPoint(double X , double Y , int classNumber , int classCount){
    this->X = X;
    this->Y = Y;
    desiredValue = new int [classCount];

    for(int i = 0 ; i < classCount ; i++){
      if(i == classNumber){
          desiredValue[i] = 1;
        }
      else{
          desiredValue[i]= -1;
        }
    }
    pointColor = colorArray[classNumber];
  }

  double X;
  double Y;
  QColor pointColor;
  int * desiredValue;

};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  int classCount;
  int currencClass = 0;
  double  * w;
  QList<MyPoint> pointList;
  QList<MyPoint> tempList;
  QList<MyPoint> normalizeList;

  virtual void paintEvent(QPaintEvent * event);
  void mousePressEvent(QMouseEvent * event);

  void clearBoard();
  void Perceptron();
  void Delta();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_6_clicked();

  void on_pushButton_4_clicked();

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

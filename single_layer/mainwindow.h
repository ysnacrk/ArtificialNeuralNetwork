#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPaintEvent>
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


struct MyPoint{
  int x;
  int y;
  QColor pointColor;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  QList<MyPoint> pointList;
  QList<MyPoint> tempList;
  QColor currentColor;

  double w[3] = {1, 1 , 0};
  bool isPerceptronDone;
  MainWindow(QWidget *parent = nullptr);

  virtual void paintEvent(QPaintEvent * event);
  void mousePressEvent(QMouseEvent * event);
  void Perceptron();
  ~MainWindow();

private slots:

  void on_redButton_clicked();

  void on_greenButton_clicked();

  void on_pushButton_clicked();

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

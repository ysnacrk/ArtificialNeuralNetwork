#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


struct MyPoint{
  double x;
  double y;
  QColor pointColor;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  QList<MyPoint> pointList;
  QList<MyPoint> tempList;
  QList<MyPoint> normalizeList;

  QColor currentColor;

  double w[3] = {2, 2 ,-1};
  bool isPerceptronDone;
  MainWindow(QWidget *parent = nullptr);

  virtual void paintEvent(QPaintEvent * event);
  void mousePressEvent(QMouseEvent * event);

  void clearBoard();
  void Perceptron();
  void Delta();

  ~MainWindow();

private slots:

  void on_redButton_clicked();

  void on_greenButton_clicked();

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

  void on_redButton_2_clicked();

  void on_greenButton_2_clicked();

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

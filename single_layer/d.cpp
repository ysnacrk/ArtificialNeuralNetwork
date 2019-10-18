#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPainter"
#include "QLabel"
#include "QPicture"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
  MyPoint myPoint;
  MyPoint tempPoint;

  myPoint.x = event->x();
  myPoint.y = event->y();

  myPoint.pointColor = currentColor;
  tempPoint.pointColor = currentColor;

  tempPoint.x = event->x() - window()->width()/2;
  tempPoint.y =  window()->height()/2 -  event->y() ;

  tempList.append(tempPoint);
  pointList.append(myPoint);

  ui->label->setNum(tempPoint.x);
  ui->label_2->setNum(tempPoint.y);

  update();
}

int sgn(double value){
  return value > 0 ? 1 : -1 ;
}

void MainWindow::Perceptron()
{
  double learningConstant = 0.1;
  double bias = 1;
  double net = 0;
  int desiredValue = 0 , output = 0;
  int error = 1;

  while(error > 0){
    error = 0;
    for(MyPoint i : tempList){

       net = w[0]*i.x + w[1]*i.y + w[2]*bias;
       output = sgn(net);

       if(i.pointColor == Qt::red){
          desiredValue = 1;
       }else{
          desiredValue = -1;
       }

        w[0] += learningConstant *(desiredValue - output) * i.x;
        w[1] += learningConstant *(desiredValue - output) * i.y;
        w[2] += learningConstant *(desiredValue - output) * bias;



        error += abs(desiredValue - output)/2;
    }
  }
  update();
}

void MainWindow::paintEvent(QPaintEvent * event)
{

  QPen paintpen(Qt::black);
  QPainter painter(this);
  paintpen.setWidth(6);
  painter.setPen(paintpen);

  for (MyPoint i : pointList) {
      QPoint p1;
      p1.setX(i.x);
      p1.setY(i.y);
      paintpen.setColor(i.pointColor);
      painter.setPen(paintpen);
      painter.drawPoint(p1);
    }
  paintpen.setColor(Qt::black);
  paintpen.setWidth(2);
  painter.setPen(paintpen);
  painter.drawLine(QLine(0 , window()->height()/2 , window()->width() , window()->height()/2));
  painter.drawLine(QLine(window()->width()/2 , 0 , window()->width()/2 , window()->height()));

  double x = -w[2] / w[0];
  double y = -w[2] / w[1];

  x += window()->width()/2;
  y = window()->height()/2 - y;
  MyPoint firstPoint , secondPoint;

  bool flag = false;

  //normalize işlemi yapmamız lazım
  for (int i = 0; i < window()->height(); i++) {
      for (int j = 0; j < window()->width(); j++) {
          i = i - window()->width()/2;
          j = window()->height()/2 -  j;
          if((w[0]*i + w[1]*j + w[2]) == 0){
              firstPoint.x = i;
              firstPoint.y = j;
              flag = true;
              break;
            }
        }
      if(flag){
          break;
       }
    }
  flag = false;
  for (int i = window()->height(); i >0 ; i--) {
      for (int j =  window()->width(); j >0; j--) {
          i = i - window()->width()/2;
          j =  window()->height()/2 -  j;
          if((w[0]*i + w[1]*j + w[2]) == 0){
              secondPoint.x = i;
              secondPoint.y = j;
              flag = true;
              break;
            }
        }
      if(flag){
          break;
       }
    }

  firstPoint.x += window()->width()/2;
  firstPoint.y =window()->height()/2 - firstPoint.y;

  secondPoint.x += window()->width()/2;
  secondPoint.y =window()->height()/2 - secondPoint.y;

  paintpen.setColor(Qt::blue);
  painter.setPen(paintpen);
  painter.drawLine(QLine(firstPoint.x , firstPoint.y , secondPoint.x , secondPoint.y));

  ui->label_5 ->setNum(window()->width()/2);
  ui->label_6 ->setNum(window()->height()/2);
  ui->label_7 ->setNum(w[2]);

}



MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_redButton_clicked()
{

    currentColor = Qt::red;
}

void MainWindow::on_greenButton_clicked()
{
    currentColor = Qt::green;
}

void MainWindow::on_pushButton_clicked()
{
    Perceptron();
}





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

  ui->label_3->setNum(tempPoint.x);
  ui->label_2->setNum(tempPoint.y);

  update();
}

void MainWindow::clearBoard()
{
  tempList.clear();
  pointList.clear();
  normalizeList.clear();
}

int sgn(double value){
  return value > 0 ? 1 : -1 ;
}

double tanh(double value){
  return (2 / (1 +  exp(-value * 2))) - 1;
}

double derivationTanh(double value){
  return 0.5* (1 - pow(value , 2));
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

void MainWindow::Delta()
{

  if(ui->checkBox->checkState()){
      double meanX = 0 , meanY = 0;

      for (MyPoint i: tempList) {
          meanX += i.x;
          meanY += i.y;
      }

      meanX /= tempList.size();
      meanY /= tempList.size();

      double differentiationX = 0 , differentiationY = 0;

      for (MyPoint i : tempList){
          differentiationX += pow((meanX - i.x) , 2);
          differentiationY += pow((meanY - i.y) , 2);
      }

      double varianceX = sqrt(differentiationX / tempList.size());
      double varianceY = sqrt(differentiationY / tempList.size());

      MyPoint tempPoint;

      for (MyPoint i : tempList) {
          tempPoint = i;
          tempPoint.x = (i.x - meanX) / varianceX;
          tempPoint.y = (i.y - meanX) / varianceY;
          tempPoint.pointColor = i.pointColor;
          normalizeList.append(tempPoint);
        }
  }

  double learningConstant = 0.1;
  double bias = 1;
  double net = 0;
  int desiredValue = 0 ;
  double output = 0;
  double derivationOutput = 0;
  double error = 0.5;

  while(error > 0.1){
    error = 0;
    for(MyPoint i : normalizeList){

       net = w[0]*i.x + w[1]*i.y + w[2]*bias;

       output =((2 / (1 + exp(-net))) - 1);
       derivationOutput = 0.5 * (1 - pow(output, 2));

       if(i.pointColor == Qt::red){
          desiredValue = 1;
       }else{
          desiredValue = -1;
       }

       //eğer desiredvalue ile output aynı ise 0 çıktısını verecek ve ağırlıklar değişmeyecek

        w[0] += learningConstant *(desiredValue - output) * i.x * derivationOutput;
        w[1] += learningConstant *(desiredValue - output) * i.y * derivationOutput;
        w[2] += learningConstant *(desiredValue - output) * bias * derivationOutput;

        error += 0.5 * pow((desiredValue - output) , 2);
    }
  }

  pointList.clear();
  for(MyPoint i : normalizeList){

      //bir koordinat işlemleri
      MyPoint temp;
      temp.x = i.x*20+ window()->width()/2;
      temp.y = window()->height()/2 - i.y*20;
      temp.pointColor = i.pointColor;
      pointList.append(temp);
    }
  normalizeList.clear();
  update();
}


void MainWindow::paintEvent(QPaintEvent * event)
{
  QPen paintpen(Qt::black);
  QPainter painter(this);
  paintpen.setWidth(6);
  painter.setPen(paintpen);

   if(ui->checkBox->checkState()){

     }
  for (MyPoint i : pointList) {
      QPoint p1;
      p1.setX((int)i.x);
      p1.setY((int)i.y);
      paintpen.setColor(i.pointColor);
      painter.setPen(paintpen);
      painter.drawPoint(p1);
    }

  paintpen.setColor(Qt::black);
  paintpen.setWidth(2);
  painter.setPen(paintpen);
  painter.drawLine(QLine(0 , window()->height()/2 , window()->width() , window()->height()/2));
  painter.drawLine(QLine(window()->width()/2 , 0 , window()->width()/2 , window()->height()));

  MyPoint firstPoint , secondPoint;

  firstPoint.x = -window()->width()/2;
  firstPoint.y = -(w[0] * firstPoint.x + w[2]*20) / w[1];

  secondPoint.x = window()->width()/2;
  secondPoint.y = -(w[0] * secondPoint.x + w[2]*20) / w[1];

  firstPoint.x += window()->width()/2;
  firstPoint.y = window()->height()/2 - firstPoint.y;

  secondPoint.x += window()->width()/2;
  secondPoint.y = window()->height()/2 - secondPoint.y;

  paintpen.setColor(Qt::blue);
  painter.setPen(paintpen);
  painter.drawLine(QLine((int)firstPoint.x , (int)firstPoint.y , (int)secondPoint.x , (int)secondPoint.y));

  ui->label_4 ->setNum(w[0]);
  ui->label_8 ->setNum(w[1]);
  ui->label_9 ->setNum(w[2]);

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





void MainWindow::on_pushButton_2_clicked()
{
    clearBoard();
    update();
}

void MainWindow::on_pushButton_3_clicked()
{
   Delta();
}

void MainWindow::on_redButton_2_clicked()
{
    currentColor = Qt::red;
}

void MainWindow::on_greenButton_2_clicked()
{
    currentColor = Qt::green;
}

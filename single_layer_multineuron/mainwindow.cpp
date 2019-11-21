#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QComboBox"
#include "QPainter"
#include "QPen"


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  classCount = 0;
  for (int i= 1; i < 9 ; i++ ) {
      ui->comboBox->addItem(QString::number(i));
  }

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  ui->comboBox_2->clear();
  QString cb = ui->comboBox ->currentText();
  classCount = 0;
  for (int i = 0; i < cb.toInt() ; i++){
      ui->comboBox_2->addItem(QString::number(i));
      classCount++;
    }

  ui->label_2->setNum(classCount);
  w = new double[classCount  * 3];

  for(int i = 0 ; i < classCount * 3 ; i++){
      w[i] = 1;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
  MyPoint myPoint(event->x() , event->y() , currencClass , classCount);
  MyPoint tempPoint(event->x() - window()->width()/2,  window()->height()/2 -  event->y() , currencClass , classCount);

  tempList.append(tempPoint);
  pointList.append(myPoint);

  ui->label_3->setNum(tempPoint.X);
  ui->label_4->setNum(tempPoint.Y);

  update();
}

void MainWindow::clearBoard()
{
  tempList.clear();
  pointList.clear();
  normalizeList.clear();
}
void MainWindow::Perceptron()
{
  double learningConstant = 0.1;
  double bias = 1;
  double * net = new double[classCount];
  int * sgn= new int[classCount];
  int error = 1;

  while(error > 0){
    error = 0;

    for(MyPoint point : tempList){
        for(int i = 0 ; i < classCount ; i ++){
            net[i] = 0;
            sgn[i] = 0;
        }
       for(int j = 0 ; j < classCount ; j++){
            for(int k = 0 ; k < 3 ; k++){
                net[j] += w[j*3] *point.X;
                net[j] += w[j*3 + 1] *point.Y;
                net[j] += w[j*3 + 2] *bias;
              }
         }

       for(int i = 0 ; i < classCount; i++){
            if(net[i] >= 0){
                sgn[i] = 1;
              }else{
                sgn[i] = -1;
              }
       }

       for(int i = 0 ; i < classCount  ; i++){
          if(sgn[i] != point.desiredValue[i]){
            w[i*3] += learningConstant *(point.desiredValue[i] - sgn[i]) * point.X;
            w[i*3 + 1] += learningConstant *(point.desiredValue[i] - sgn[i]) * point.Y;
            w[i*3 + 2] += learningConstant *(point.desiredValue[i] - sgn[i]) * bias;
          }
          error += abs(point.desiredValue[i]- sgn[i])/2;
       }
    }
  }
  update();
}

void MainWindow::Delta()
{
  if(ui->checkBox->checkState()){
      double meanX = 0 , meanY = 0;

      for (MyPoint i: tempList) {
          meanX += i.X;
          meanY += i.Y;
      }

      meanX /= tempList.size();
      meanY /= tempList.size();

      double differentiationX = 0 , differentiationY = 0;

      for (MyPoint i : tempList){
          differentiationX += pow((meanX - i.X) , 2);
          differentiationY += pow((meanY - i.Y) , 2);
      }

      double varianceX = sqrt(differentiationX / tempList.size());
      double varianceY = sqrt(differentiationY / tempList.size());

      MyPoint tempPoint(1,1,1,classCount);

      for (MyPoint i : tempList) {
          tempPoint = i;
          tempPoint.X = (i.X - meanX) / varianceX;
          tempPoint.Y = (i.Y - meanX) / varianceY;
          tempPoint.pointColor = i.pointColor;
          for(int j = 0 ; j < classCount ; j++){
              tempPoint.desiredValue[j] = i.desiredValue[j];
          }
          normalizeList.append(tempPoint);
        }
  }

  double learningConstant = 0.1;
  double bias = 1;
  double * net = new double[classCount];
  double * derivation = new double[classCount];
  int * activation = new int[classCount];
  double error = 0.5;

  while(error > 0.1){
      error = 0;
      for(MyPoint point : normalizeList){
          for(int i = 0 ; i < classCount ; i ++){
              net[i] = 0;
              activation[i] = 0;
              derivation[i] = 0;
          }

      for(int j = 0 ; j < classCount ; j++){
           for(int k = 0 ; k < 3 ; k++){
               net[j] += w[j*3] *point.X;
               net[j] += w[j*3 + 1] *point.Y;
               net[j] += w[j*3 + 2] *bias;
             }
        }

      for(int i = 0 ; i < classCount; i++){
          activation[i] = ((2 / (1 + exp(-net[i]))) - 1);
      }
      for(int i = 0 ; i < classCount; i++){
          derivation[i] = 0.5 * (1 - pow(activation[i], 2));
      }

      for(int i = 0 ; i < classCount  ; i++){

           w[i*3] += learningConstant *(point.desiredValue[i] - activation[i]) * point.X * derivation[i];
           w[i*3 + 1] += learningConstant *(point.desiredValue[i] - activation[i]) * point.Y * derivation[i];
           w[i*3 + 2] += learningConstant *(point.desiredValue[i] - activation[i]) * bias * derivation[i];

         error += 0.5 * pow((point.desiredValue[i]  - activation[i]) , 2);
      }

     }
  }

  pointList.clear();
  for(MyPoint i : normalizeList){

      //bir koordinat işlemleri
      MyPoint temp(1,1,1,classCount);
      temp.X = i.X*100+ window()->width()/2;
      temp.Y = window()->height()/2 - i.Y*100;
      temp.pointColor = i.pointColor;
      for(int j = 0 ; j < classCount ; j++){
          temp.desiredValue[j] = i.desiredValue[j];
      }
      pointList.append(temp);
    }
  normalizeList.clear();
  update();

}


void MainWindow::paintEvent(QPaintEvent *event)
{
  QPen paintpen(Qt::black);
  QPainter painter(this);
  paintpen.setWidth(6);
  painter.setPen(paintpen);
  int expand = 1;
  for (MyPoint i : pointList) {
      QPoint p1;
      p1.setX((int)i.X);
      p1.setY((int)i.Y);
      paintpen.setColor(i.pointColor);
      painter.setPen(paintpen);
      painter.drawPoint(p1);
    }

  paintpen.setColor(Qt::black);
  paintpen.setWidth(2);
  painter.setPen(paintpen);
  painter.drawLine(QLine(0 , window()->height()/2 , window()->width() , window()->height()/2));
  painter.drawLine(QLine(window()->width()/2 , 0 , window()->width()/2 , window()->height()));

  if(ui->checkBox->checkState()){
      expand = 100;
    }
  for(int i = 0 ; i < classCount ; i++){

      MyPoint firstPoint(0,0,0,classCount) , secondPoint(0,0,0,classCount);

      firstPoint.X= -window()->width()/2;
      firstPoint.Y = -(w[i*3] * firstPoint.X + w[i*3+2]*expand) / w[i*3 +1];

      secondPoint.X = window()->width()/2;
      secondPoint.Y = -(w[i*3] * secondPoint.X + w[i*3 + 2]*expand) / w[i*3 + 1];

      firstPoint.X += window()->width()/2;
      firstPoint.Y = window()->height()/2 - firstPoint.Y;

      secondPoint.X += window()->width()/2;
      secondPoint.Y = window()->height()/2 - secondPoint.Y;

      paintpen.setColor(colorArray[i]);
      painter.setPen(paintpen);
      painter.drawLine(QLine((int)firstPoint.X, (int)firstPoint.Y , (int)secondPoint.X , (int)secondPoint.Y));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString cb = ui->comboBox_2 ->currentText();
    currencClass = cb.toInt();
}

void MainWindow::on_pushButton_3_clicked()
{
    Perceptron();
}

void MainWindow::on_pushButton_6_clicked()
{
    Delta();
}

void MainWindow::on_pushButton_4_clicked()
{
    normalizeList.clear();
    pointList.clear();
    tempList.clear();
    update();
}

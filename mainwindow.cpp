#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionConnect_triggered()
{
    emit findDevice();
}
/*
void MainWindow::on_display_resized()
{
    emit displayResized(this->findChild<Visual *>("display")->size());
}*/

void MainWindow::on_deleteAllButton_clicked()
{
    emit deleteAll();
}

void MainWindow::on_buttonDelObject_clicked()
{
    std::cout << "Index Selected : " << indexSelected << std::endl;
    emit deleteSelected();
}


void MainWindow::on_buttonAddRigid_clicked()
{
    emit addRigidBody();
}

void MainWindow::on_buttonAddElastic_clicked()
{
    emit addElasticBody();
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    emit newStiffness(arg1);
}

void MainWindow::on_ledStateBox_toggled(bool checked)
{
    emit ledState(checked);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <connectiondialog.h>
#include <body.h>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionConnect_triggered();

    //void on_display_resized();
    void on_deleteAllButton_clicked();
    void on_buttonDelObject_clicked();


    void on_buttonAddRigid_clicked();

    void on_buttonAddElastic_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_ledStateBox_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

    void on_actionDisconnect_triggered();

    void deviceConnected();

    void deviceDisconnected();

signals:
    //void displayResized(QSize newSize);
    void findDevice();
    void addRigidBody();
    void addElasticBody();
    void deleteSelected();
    void deleteAll();
    void selectedObject(int index);
    void newStiffness(float selected);
    void ledState(bool);
    void newOffset(int offset);
    void disconnectDevice();

private:
    Ui::MainWindow *ui;
    int indexSelected;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define FPGA_DOT_DEVICE "/dev/fpga_dot"
#define FPGA_STEP_MOTOR_DEVICE "/dev/fpga_step_motor"
#define FPGA_TEXT_LCD_DEVICE "/dev/fpga_text_lcd"
#define FPGA_PUSH_SWITCH_DEVICE "/dev/fpga_push_switch"
#define LED_DEVICE "/dev/fpga_led"

#define MAX_BUFF 32
#define MAX_BUTTON 9
#define LINE_BUFF 16



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
    void on_pushButton_clicked();


    void on_Close_BT_clicked();

private:
    Ui::MainWindow *ui;
    QMessageBox msgBox;
};

#endif // MAINWINDOW_H

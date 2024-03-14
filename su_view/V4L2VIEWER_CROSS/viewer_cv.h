#ifndef VIEWER_CV_H
#define VIEWER_CV_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h> 				//for device open flags
#include <sys/mman.h> 			//for mmap flags
#include <sys/ioctl.h> 			//for ioctl
#include <linux/videodev2.h> 	//for v4l2 structs, enums, etc..

#include <QMessageBox>
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <QMainWindow>

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define DEVNAME         0
#define CAMERA_WIDTH    1920
#define CAMERA_HEIGHT   1080  //1280, 800
#define NBUF            4

QT_BEGIN_NAMESPACE
namespace Ui { class Viewer_cv; }
QT_END_NAMESPACE

class Viewer_cv : public QMainWindow
{
    Q_OBJECT

public:
    Viewer_cv(QWidget *parent = nullptr);
    ~Viewer_cv();

private slots:
    void on_OPEN_clicked();
    void on_START_STOP_clicked();
    void loop();

private:
    Ui::Viewer_cv *viewer_ui;
    cv::VideoCapture *capture = new cv::VideoCapture();
    cv::Mat frame;
    QTimer *timer = new QTimer(this);
    int stop_flag = false;
};
#endif // VIEWER_CV_H

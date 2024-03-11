#ifndef V4L2_VIEWER_H
#define V4L2_VIEWER_H

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
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define DEVNAME "/dev/video0"
#define CAMERA_WIDTH    1920
#define CAMERA_HEIGHT   1080
#define NBUF            4

QT_BEGIN_NAMESPACE
namespace Ui { class V4L2_viewer; }
QT_END_NAMESPACE

class V4L2_viewer : public QMainWindow
{
    Q_OBJECT

public:
    V4L2_viewer(QWidget *parent = nullptr);
    ~V4L2_viewer();

public slots:
    void loop();

protected:
    int fd;
    QTimer *timer = new QTimer(this);

private slots:
    void on_START_STOP_clicked();
    void on_OPEN_clicked();

    void on_CLOSE_clicked();

private:
    Ui::V4L2_viewer *ui_viewer;
    //unsigned char* beforeLOOP();
    int beforeLOOP();
    //unsigned char* buffers;
    unsigned char* buffers_[NBUF];
    int stop_flag = false;

};
#endif // V4L2_VIEWER_H

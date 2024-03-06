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
#include <QThread>
#include <QMutex>

#ifndef V4L2THREAD_H
#define V4L2THREAD_H
#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define DEVNAME "/dev/video0"
#define CAMERA_WIDTH    1920
#define CAMERA_HEIGHT   1080
#define NBUF            3

class v4l2Thread : public QThread
{
    Q_OBJECT
public:
    explicit v4l2Thread(QObject* parent = nullptr);
    int m_stopFlag;
public slots:
    void stop();
private:
    void run(int fd, unsigned char** buF);
    QString debug_str;
signals:
    void display_sig(QImage* frame);
    void qdebug_sig(QString debug_str);
};

#endif // V4L2THREAD_H

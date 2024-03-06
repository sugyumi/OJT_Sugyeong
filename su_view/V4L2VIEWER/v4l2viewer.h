#include "v4l2thread.h"

/*
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
*/

#ifndef V4L2VIEWER_H
#define V4L2VIEWER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class V4L2viewer; }
QT_END_NAMESPACE

class V4L2viewer : public QMainWindow
{
    Q_OBJECT
public:
    V4L2viewer(QWidget *parent = nullptr);
    ~V4L2viewer();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();

public slots:
    void display_slot(QImage* frame);
    void qdebug_slot(QString debug_str);

private:
    Ui::V4L2viewer *viewerui;
    v4l2Thread *v4l2thread;

signals:
    void v4l2ThreadStop();
};
#endif // V4L2VIEWER_H

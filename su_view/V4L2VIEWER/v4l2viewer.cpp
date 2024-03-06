#include "v4l2viewer.h"
#include "ui_v4l2viewer.h"

V4L2viewer::V4L2viewer(QWidget *parent)
    : QMainWindow(parent)
    , viewerui(new Ui::V4L2viewer)
{
    viewerui->setupUi(this);
    v4l2thread = new v4l2Thread();
    connect(this, SIGNAL(v4l2ThreadStop()), v4l2thread, SLOT(stop()));
    connect(v4l2thread, SIGNAL(display_sig(QImage*)), this, SLOT(display_slot(QImage*)));
    connect(v4l2thread, SIGNAL(qdebug_sig(QString)), this, SLOT(qdebug_slot(QString)));
}

V4L2viewer::~V4L2viewer()
{
    v4l2thread->stop();
    delete viewerui;
}

void V4L2viewer::display_slot(QImage* frame)
{
    QPixmap *pix = new QPixmap();
    *pix = QPixmap::fromImage(*frame, Qt::AutoColor);
    *pix = pix->scaled(800, 600);
    viewerui->Cam->setPixmap(*pix);
}

void V4L2viewer::qdebug_slot(QString debug_str)
{
    qDebug() << debug_str;
    v4l2thread->quit();
    qDebug() << "*****  Thread is exit  *****";
}

void V4L2viewer::on_Start_clicked()
{
    viewerui->Stop->setEnabled(true);
    viewerui->Start->setEnabled(false);
    v4l2thread->start();
    qDebug() << "*****  Thread is running :" << v4l2thread->isRunning() << " *****";
}

void V4L2viewer::on_Stop_clicked()
{
    viewerui->Stop->setEnabled(false);
    viewerui->Start->setEnabled(true);
    emit v4l2ThreadStop();
    qDebug() << "*****  Thread is finised :" << v4l2thread->isFinished() << " *****";
}

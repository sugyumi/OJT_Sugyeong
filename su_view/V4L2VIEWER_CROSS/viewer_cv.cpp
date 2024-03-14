#include "viewer_cv.h"
#include "ui_viewer_cv.h"

Viewer_cv::Viewer_cv(QWidget *parent)
    : QMainWindow(parent)
    , viewer_ui(new Ui::Viewer_cv)
{
    viewer_ui->setupUi(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
}

Viewer_cv::~Viewer_cv()
{
    delete viewer_ui;
}

void Viewer_cv::on_OPEN_clicked()
{
    int api = cv::CAP_ANY;
    capture->open(DEVNAME, api);

    if(!capture->isOpened())
    {
        qDebug() << "Can not OPEN DEVICE";
        QMessageBox::warning(this, "Warning", "Can not OPEN DEVICE");
        return;
    }
    viewer_ui->START_STOP->setEnabled(true);
    viewer_ui->OPEN->setEnabled(false);
}

void Viewer_cv::on_START_STOP_clicked()
{
    if (!stop_flag)
    {
        stop_flag = true;
        viewer_ui->START_STOP->setText("STOP");
        timer->start(33);
    }
    else
    {
        stop_flag = false;
        viewer_ui->START_STOP->setText("START");
        timer->stop();
    }
}

void Viewer_cv::loop()
{
    capture->read(frame);
    if(frame.empty())
    {
        qDebug() << "Frame is Empty";
    }
    else
    {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage img = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        img = img.scaled(viewer_ui->CAMERA->size(), Qt::KeepAspectRatio);
        viewer_ui->CAMERA->setPixmap(QPixmap::fromImage(img));
    }
}

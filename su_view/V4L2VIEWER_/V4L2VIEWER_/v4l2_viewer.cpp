#include "v4l2_viewer.h"
#include "ui_v4l2_viewer.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define DEVNAME "/dev/video0"
#define CAMERA_WIDTH    1920
#define CAMERA_HEIGHT   1080
#define NBUF            3
//#define OPT

V4L2_viewer::V4L2_viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui_viewer(new Ui::V4L2_viewer)
{
    ui_viewer->setupUi(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));

}

V4L2_viewer::~V4L2_viewer()
{
    delete ui_viewer;
}

void V4L2_viewer::on_OPEN_clicked()
{
    buffers = beforeLOOP();
    if (buffers != 0)
    {
        ui_viewer->START_STOP->setEnabled(true);
        ui_viewer->CLOSE->setEnabled(true);
        ui_viewer->START_STOP->setText("START");
        ui_viewer->OPEN->setEnabled(false);
    }
}


void V4L2_viewer::on_START_STOP_clicked()
{
    if (!stop_flag)
    {
        /* start */
        stop_flag = true;
        ui_viewer->START_STOP->setText("STOP");
        //loop(buffers);
        timer->start(33);
    }
    else
    {
        /* stop */
        stop_flag = false;
        ui_viewer->START_STOP->setText("START");
        timer->stop();
    }
}

void V4L2_viewer::on_CLOSE_clicked()
{
    unsigned int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd, VIDIOC_STREAMOFF, &type) == -1){
        qDebug() << "***** Cannot STREAM OFF";
        return;
    }
    ::close(fd);
    ui_viewer->START_STOP->setEnabled(false);
    ui_viewer->OPEN->setEnabled(true);
}

unsigned char* V4L2_viewer::beforeLOOP()
{
    fd = open(DEVNAME, O_RDWR);
    if (fd < 0) {
        qDebug() << "***** Cannot Open Devic *****";
        QMessageBox::warning(this, "Warning", "Can not OPEN DEVICE");
        return 0;
    }
    else {
        /* CAP */
        struct v4l2_capability cap;
        if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
            qDebug() << "***** Query capabilites *****";
            QMessageBox::warning(this, "Warning", "Can not QUERYCAP");
            return 0;
            //exit(EXIT_FAILURE);
        }
    }

    /* SET FORMAT */
    struct v4l2_format fmt;
    CLEAR(fmt);

    fmt.type 				= V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
    fmt.fmt.pix.width 		= CAMERA_WIDTH;
    fmt.fmt.pix.height 		= CAMERA_HEIGHT;
    #ifdef OPT
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    #else
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;
    #endif

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
    {
        qDebug() << "***** Cannot Set Format *****";
        QMessageBox::warning(this, "Warning", "Can not SET FORMAT");
        return 0;
    }
    printf("Success to set format\n");

    /*           REQBUFS         */
    /* Inform BUFFERs to device */
    struct v4l2_requestbuffers req;
    CLEAR(req);

    req.count = NBUF;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1)
    {
        qDebug() << "***** Cannot REQ BUF *****";
        QMessageBox::warning(this, "Warning", "Can not REQ BUF");
        return 0;
    }
    printf("Success to request buffers\n");


    /* Create buffer set */
    unsigned char* buffers[req.count];


    /*                  QUERYBUFS                    */
    /* Get BUFFERs Offset and Length & Alloc BUFFERs */
    unsigned int buffers_idx;
    for (buffers_idx = 0; buffers_idx < req.count; buffers_idx++)  /* req.count == NBUF*/
    {
        struct v4l2_buffer buf;  /* Only used for getting offset & length */
        CLEAR(buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = buffers_idx;

        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)
        {
            qDebug() << "***** Cannot QUERY Buffers *****";
            QMessageBox::warning(this, "Warning", "Can not QUERY BUF");
            return 0;
        }


        /* mmap */
        buffers[buffers_idx] = (u_int8_t*)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

        printf("mmap offset %d\n", buf.m.offset);

        if (MAP_FAILED == buffers[buffers_idx])
        {
            std::cout << buffers[buffers_idx] << "\n";
        }
        printf("Success to mmap buffer %d\n", buffers_idx);
    }


    /* QBUF */
    for (unsigned int i=0; i<req.count; ++i)
    {
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
        {
            qDebug() << "***** Cannot QUE Buffers *****";
            QMessageBox::warning(this, "Warning", "Can not QUE BUF_1");
            return 0;
        }
    }


    /* STREAM ON */
    unsigned int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1)
    {
        qDebug() << "***** Cannot Stream on *****";
        QMessageBox::warning(this, "Warning", "Can not STREAM ON");
        return 0;
    }
    printf("Stream on: %s\n", DEVNAME);

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval tv;
    CLEAR(tv);
    tv.tv_sec = 2;

    if(select(fd+1, &fds, NULL, NULL, &tv) == -1){
        qDebug() << "***** Waiting for Frame *****";
        QMessageBox::warning(this, "Warning", "Wating for Frame");
        return 0;
    }

    return *buffers;
}

void V4L2_viewer::loop()
{
    qDebug() << "==========LOOPSTART===========";
    struct v4l2_buffer buf;
    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    /* DQBUF */
    if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
    {
        qDebug() << "***** Cannot DQUE Buffer *****";
        exit(1);
    }
    //printf("%s get buf index %d \n", DEVNAME, buf.index);

#if 1  /* use opencv */
    //CvMat frame = cvMat(600, 800, CV_8UC1, &buffers[buf.index]);
    //CvMat frame = cvMat(600, 800, CV_8UC1);
    //frame.data = buffers[buf.index];
    //CvMat temp;
   // cvCvtColor(frame, temp, cv::COLOR_BGR2RGB);

    cv::Mat frame = cv::Mat(cv::Size(800, 600), CV_8UC2, &buffers[buf.index]);
    cv::Mat bgr_frame = cv::Mat(cv::Size(800, 600), CV_8UC3);
    cv::Mat rgb_frame = cv::Mat(cv::Size(800, 600), CV_8UC3);
    cv::cvtColor(frame, bgr_frame, cv::COLOR_YUV2RGB_UYVY);
    cv::cvtColor(bgr_frame, rgb_frame, cv::COLOR_BGR2RGB);


    //CvMat cvmat2 = cvMat(cvSize(800, 600), CV_8UC2, &buffers[buf.index]);
    //CvMat temp;
    //cv::cvtColor(frame.data, temp, cv::COLOR_RGB2YUV);

    QImage img = QImage((const uchar*)rgb_frame.data, rgb_frame.cols, rgb_frame.rows, rgb_frame.step, QImage::Format_RGB888);
    QPixmap *pix = new QPixmap();
    *pix = QPixmap::fromImage(img.rgbSwapped(), Qt::AutoColor);
    ui_viewer->CAMERA->setPixmap(*pix);

#endif
#if 0 /* not use opencv */
    QImage *frame = new QImage(&buffers[buf.index], 800, 600, QImage::Format_RGB888);
    QPixmap *pix = new QPixmap();
    *pix = QPixmap::fromImage(*frame, Qt::AutoColor);
    *pix = pix->scaled(800, 600);
    ui_viewer->CAMERA->setPixmap(*pix);
#endif

    /* QBUF */
    if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
    {
        qDebug() << "***** Cannot QUE Buffer *****";
        exit(1);
    }
    //printf("%s set buf index %d \n", DEVNAME, buf.index);

     qDebug() << "==========LOOPEND===========";
}


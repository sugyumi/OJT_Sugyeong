#include "v4l2thread.h"

v4l2Thread::v4l2Thread(QObject*parent):
    QThread(parent)
{
    //initialize parameters
    m_stopFlag=false;
}

void v4l2Thread::run(int fd, unsigned char** buF) {

    debug_str = "test";
    emit qdebug_sig(debug_str);
    fd = open(DEVNAME, O_RDWR);
    if (fd < 0) {
        debug_str = "***** Cannot Open Devic *****";
        emit qdebug_sig(debug_str);
    }
    else {
        //viewerui->Start->setEnabled(true);
        /* CAP */
        struct v4l2_capability cap;
        if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
            perror("Query capabilites");
            exit(EXIT_FAILURE);
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
        debug_str = "***** Cannot Set Format *****";
        emit qdebug_sig(debug_str);
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
        debug_str = "***** Cannot REQ BUF *****";
        emit qdebug_sig(debug_str);
    }
    printf("Success to request buffers\n");


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

        ioctl(fd, VIDIOC_QUERYBUF, &buf);


        /* mmap */
        buF[buffers_idx] = (u_int8_t*)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

        printf("mmap offset %d\n", buf.m.offset);

        if (MAP_FAILED == buF[buffers_idx])
        {
            std::cout << buF[buffers_idx] << "\n";
        }
        printf("Success to mmap buffer %d\n", buffers_idx);
    }


    /* QBUF */
    for (unsigned int i=0; i<req.count+1; ++i)
    {
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
        {
            debug_str = "***** Cannot QUE Buffers *****";
            emit qdebug_sig(debug_str);
        }
    }


    /* STREAM ON */
    unsigned int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1)
    {
        qDebug() << "***** Cannot Stream on *****";
        exit(EXIT_FAILURE);
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
        exit(1);
        //exit(EXIT_FAILURE);
    }


    /* loop - DQ & Q */
    while (!m_stopFlag) {

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
        printf("%s get buf index %d \n", DEVNAME, buf.index);
    #if 1
        QImage *frame = new QImage(buF[buf.index], 800, 600, QImage::Format_RGB888);
        emit display_sig(frame);
        usleep(330000);
    #endif
        /* QBUF */
        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
        {
            qDebug() << "***** Cannot QUE Buffer *****";
            exit(1);
        }
        printf("%s set buf index %d \n", DEVNAME, buf.index);
    }
    qDebug() << "loop end\n";
}

void v4l2Thread::stop() {
    m_stopFlag = true;
}

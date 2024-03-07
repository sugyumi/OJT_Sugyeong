#include "v4l2_viewer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    V4L2_viewer w;
    w.show();
    return a.exec();
}

#include "v4l2viewer.h"
#include "v4l2thread.h"

#include <QGuiApplication>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    V4L2viewer w;
    w.show();
    return a.exec();
}

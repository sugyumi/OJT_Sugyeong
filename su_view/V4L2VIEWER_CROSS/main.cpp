#include "viewer_cv.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Viewer_cv w;
    w.show();
    return a.exec();
}

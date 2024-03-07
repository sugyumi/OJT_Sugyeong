/********************************************************************************
** Form generated from reading UI file 'v4l2_viewer.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_V4L2_VIEWER_H
#define UI_V4L2_VIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_V4L2_viewer
{
public:
    QWidget *centralwidget;
    QLabel *CAMERA;
    QPushButton *START_STOP;
    QPushButton *OPEN;
    QPushButton *CLOSE;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *V4L2_viewer)
    {
        if (V4L2_viewer->objectName().isEmpty())
            V4L2_viewer->setObjectName(QString::fromUtf8("V4L2_viewer"));
        V4L2_viewer->resize(981, 654);
        centralwidget = new QWidget(V4L2_viewer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CAMERA = new QLabel(centralwidget);
        CAMERA->setObjectName(QString::fromUtf8("CAMERA"));
        CAMERA->setGeometry(QRect(160, 10, 800, 600));
        CAMERA->setAutoFillBackground(false);
        CAMERA->setFrameShape(QFrame::Box);
        START_STOP = new QPushButton(centralwidget);
        START_STOP->setObjectName(QString::fromUtf8("START_STOP"));
        START_STOP->setEnabled(false);
        START_STOP->setGeometry(QRect(10, 120, 140, 100));
        OPEN = new QPushButton(centralwidget);
        OPEN->setObjectName(QString::fromUtf8("OPEN"));
        OPEN->setGeometry(QRect(10, 10, 140, 100));
        CLOSE = new QPushButton(centralwidget);
        CLOSE->setObjectName(QString::fromUtf8("CLOSE"));
        CLOSE->setEnabled(false);
        CLOSE->setGeometry(QRect(40, 550, 75, 50));
        V4L2_viewer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(V4L2_viewer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 981, 22));
        V4L2_viewer->setMenuBar(menubar);
        statusbar = new QStatusBar(V4L2_viewer);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        V4L2_viewer->setStatusBar(statusbar);

        retranslateUi(V4L2_viewer);

        QMetaObject::connectSlotsByName(V4L2_viewer);
    } // setupUi

    void retranslateUi(QMainWindow *V4L2_viewer)
    {
        V4L2_viewer->setWindowTitle(QCoreApplication::translate("V4L2_viewer", "V4L2_viewer", nullptr));
        CAMERA->setText(QString());
        START_STOP->setText(QCoreApplication::translate("V4L2_viewer", "START/STOP", nullptr));
        OPEN->setText(QCoreApplication::translate("V4L2_viewer", "OPEN", nullptr));
        CLOSE->setText(QCoreApplication::translate("V4L2_viewer", "CLOSE", nullptr));
    } // retranslateUi

};

namespace Ui {
    class V4L2_viewer: public Ui_V4L2_viewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_V4L2_VIEWER_H

#-------------------------------------------------
#
# Project created by QtCreator 2016-08-28T17:46:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionEstimation_ParameterTuning_Gui
TEMPLATE = app

INCLUDEPATH += /usr/local/include /home/mehmetali/workspace/EgoMotionAnalysis/src
LIBS += -L/home/mehmetali/workspace/EgoMotionAnalysis/Release -lEgoMotionAnalysis -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_optflow -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    banksampahcontroller.cpp \
    graph.cpp \
    hashtable.cpp \
    main.cpp \
    mainwindow.cpp \
    pageantrean.cpp \
    pagedashboard.cpp \
    pagegudang.cpp \
    pagenasabah.cpp \
    pagepengiriman.cpp \
    pagepeta.cpp \
    pagewilayah.cpp \
    queue.cpp \
    searching.cpp \
    sorting.cpp \
    stack.cpp \
    tree.cpp

HEADERS += \
    DataStructures.h \
    banksampahcontroller.h \
    graph.h \
    hashtable.h \
    mainwindow.h \
    pageantrean.h \
    pagedashboard.h \
    pagegudang.h \
    pagenasabah.h \
    pagepengiriman.h \
    pagepeta.h \
    pagewilayah.h \
    queue.h \
    searching.h \
    sorting.h \
    stack.h \
    tree.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

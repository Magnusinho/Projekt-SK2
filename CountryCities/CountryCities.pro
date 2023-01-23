QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    createroom.cpp \
    findrooms.cpp \
    gettingans.cpp \
    lobby.cpp \
    main.cpp \
    showplayersans.cpp \
    startgame.cpp \
    totalscore.cpp

HEADERS += \
    createroom.h \
    findrooms.h \
    gettingans.h \
    lobby.h \
    showplayersans.h \
    startgame.h \
    totalscore.h

FORMS += \
    createroom.ui \
    findrooms.ui \
    gettingans.ui \
    lobby.ui \
    showplayersans.ui \
    startgame.ui \
    totalscore.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CentralView.cpp \
    FileCheck.cpp \
    FileView.cpp \
    MainWindow.cpp \
    BrowserView.cpp \
    ConnectionWidget.cpp \
    SettingsView.cpp \
    main.cpp \
    QsqlConnectionDialog.cpp

HEADERS += \
    CentralView.h \
    FileCheck.h \
    FileView.h \
    MainWindow.h \
    BrowserView.h \
    ConnectionWidget.h \
    QsqlConnectionDialog.h \
    SettingsView.h

FORMS += \
    CentralView.ui \
    FileView.ui \
    MainWindow.ui \
    BrowserView.ui \
    QsqlConnectionDialog.ui \
    SettingsView.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CentralView.cpp \
    file/FileCheck.cpp \
    file/FileCheckModel.cpp \
    file/FileView.cpp \
    MainWindow.cpp \
    browser/BrowserView.cpp \
    browser/ConnectionWidget.cpp \
    settings/Settings.cpp \
    settings/SettingsView.cpp \
    main.cpp \
    browser/QsqlConnectionDialog.cpp \
    support/MainWindowExt.cpp \
    support/controls/StatusIndicator.cpp \
    support/controls/WaitingSpinner.cpp \
    support/dialog/WaitDialog.cpp \
    support/style/CustomStyle.cpp

HEADERS += \
    CentralView.h \
    file/FileCheck.h \
    file/FileCheckModel.h \
    file/FileView.h \
    MainWindow.h \
    browser/BrowserView.h \
    browser/ConnectionWidget.h \
    browser/QsqlConnectionDialog.h \
    settings/Settings.h \
    settings/SettingsView.h \
    support/MainWindowExt.h \
    support/controls/StatusIndicator.h \
    support/controls/WaitingSpinner.h \
    support/dialog/WaitDialog.h \
    support/style/CustomStyle.h

FORMS += \
    CentralView.ui \
    file/FileView.ui \
    MainWindow.ui \
    browser/BrowserView.ui \
    browser/QsqlConnectionDialog.ui \
    settings/SettingsView.ui \
    support/about/AboutDialog.ui

RESOURCES += \
    resources/style/DarkStyle.qrc \
    resources/style/LightStyle.qrc \
    resources/resource.qrc

RC_FILE += \
    resources/resource.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

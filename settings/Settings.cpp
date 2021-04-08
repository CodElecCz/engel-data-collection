#include "Settings.h"

#include <QCoreApplication>
#include <QFileInfo>

Settings::Settings()
{

}

QString Settings::iniFilePath()
{
    QString iniFilePath = QFileInfo(QCoreApplication::applicationFilePath()).filePath();
    iniFilePath.replace(".exe", ".ini");

    return iniFilePath;
}

QString Settings::logFilePath()
{
    QString logFilePath = QFileInfo(QCoreApplication::applicationFilePath()).filePath();
    logFilePath.replace(".exe", ".log");

    return logFilePath;
}

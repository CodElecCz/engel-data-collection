#include "Settings.h"

#include <QCoreApplication>
#include <QFileInfo>

Settings::Settings()
{

}

QString Settings::iniFilePath()
{
    QString configFilePath = QFileInfo(QCoreApplication::applicationFilePath()).filePath();
    configFilePath.replace(".exe", ".ini");

    return configFilePath;
}

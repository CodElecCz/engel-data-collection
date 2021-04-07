#include "FileCheck.h"

#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QDir>

FileCheck::FileCheck(const QString& path, QObject* parent) :
    QObject(parent),
    m_path(path)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
}

void FileCheck::start(int timeMs)
{
    m_timer.start(timeMs);
}

void FileCheck::stop()
{
    m_timer.stop();
}

bool FileCheck::fileExist() const
{
    QFileInfo fileInfo(m_path);

    return fileInfo.exists();
}

void FileCheck::timer_timeout()
{    
    if(fileExist())
    {
        SFileCheckInfo info  = getFileInfo();
    }
}

QString FileCheck::fileName() const
{
    QFileInfo fileInfo(m_path);

    return fileInfo.fileName();
}

QString FileCheck::filePath() const
{
    QFileInfo fileInfo(m_path);

    return fileInfo.filePath();
}

QString FileCheck::fileDir() const
{
    QFileInfo fileInfo(m_path);

    return fileInfo.absolutePath();
}

QDateTime FileCheck::fileModified() const
{
    QFileInfo fileInfo(m_path);

    return fileInfo.lastModified();
}

FileCheck::SFileCheckInfo FileCheck::getFileInfo() const
{
    FileCheck::SFileCheckInfo info;

    QFileInfo fileInfo(m_path);

    info.valid = fileInfo.exists();

    if(info.valid)
    {
        info.fileSize = fileInfo.size();
        info.lastModified = fileInfo.lastModified();

        QFile file(m_path);
        file.copy(m_path + ".check");

        QFile fileCheck(m_path + ".check");
        if(!fileCheck.open(QIODevice::ReadOnly))
        {
            //error
            qDebug() << "Unable to open file: " << m_path;
        }
        else
        {
            QTextStream in(&fileCheck);

            int row = -1;
            while(!in.atEnd())
            {
                QString line = in.readLine();
                if(row<0)
                {
                    //parse header
                    info.header = line.split(",");
                    row = 0;
                }
                else
                {
                    //record
                    row++;
                }
            }
            file.close();

            info.nbOfRecords = row;

            fileCheck.remove();
        }
    }

    return info;
}

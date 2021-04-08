#include "FileCheck.h"

#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QDir>

FileCheck::FileCheck(const QString& path, int index, int machineId, QObject* parent) :
    QObject(parent),
    m_path(path),
    m_fileInfo(),
    m_index(index),
    m_machineId(machineId),
    m_checkTime(),
    m_timeMs(10000)
{
    m_delay.setSingleShot(true);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    connect(&m_delay, SIGNAL(timeout()), this, SLOT(delay_start()));
}

QDateTime FileCheck::fileCheckTime() const
{
    return m_checkTime;
}

void FileCheck::start(int timeMs, int delayMs)
{
    m_timeMs = timeMs;

    if(delayMs<=0)
    {
        delay_start();
    }
    else
        m_delay.start(delayMs);
}

void FileCheck::stop()
{
    m_delay.stop();
    m_timer.stop();
}

bool FileCheck::fileExist() const
{
    QFileInfo fileInfo(m_path);

    return fileInfo.exists();
}

void FileCheck::delay_start()
{
    timer_timeout();
    m_timer.start(m_timeMs);
}

void FileCheck::timer_timeout()
{        
    SFileCheckInfo info  = getFileInfo();

    if(!(info == m_fileInfo))
    {
        m_fileInfo = info;

        qInfo() << "FileCheck change, machine_id: " << m_machineId;
    }

    m_checkTime = QDateTime::currentDateTime();

    emit fileChanged(m_index);
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

int FileCheck::fileSize() const
{
    QFileInfo fileInfo(m_path);

    return fileInfo.size();
}

QDateTime FileCheck::fileModified() const
{
    QFileInfo fileInfo(m_path);

    return fileInfo.lastModified();
}

int FileCheck::machineId() const
{
    return m_machineId;
}

FileCheck::SFileCheckInfo FileCheck::getFileInfo() const
{
    SFileCheckInfo info;

    QFileInfo fileInfo(m_path);

    info.exists = fileInfo.exists();

    if(info.exists)
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

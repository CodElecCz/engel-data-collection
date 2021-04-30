#include "FileCheck.h"
#include "settings/Settings.h"

#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QException>
#include <QSqlError>
#include <QSqlQuery>

namespace
{
    const int cInsert_MaxNbOfRecords(1000);
}

FileCheck::FileCheck(const QString& path, int index, int machineId, QObject* parent) :
    QObject(parent),
    m_path(path),
    m_fileInfo(),
    m_index(index),
    m_machineId(machineId),
    m_checkTime(),
    m_timeMs(10000),
    m_db()
{
    m_delay.setSingleShot(true);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    connect(&m_delay, SIGNAL(timeout()), this, SLOT(delay_start()));

     Settings::readLastInsert(QString::number(m_machineId), m_fileInfo.lastInserted);
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

void FileCheck::delay_start()
{    
    m_timer.start(m_timeMs);
}

QString FileCheck::fileName() const
{
    QFileInfo fileInfo(m_path);
    return fileInfo.fileName();
}

QString FileCheck::filePath() const
{
    //QFileInfo fileInfo(m_path);
    //return fileInfo.filePath();

    return m_path;
}

QString FileCheck::fileDir() const
{
    QFileInfo fileInfo(m_path);
    return fileInfo.absolutePath();
}

int FileCheck::fileSize() const
{
    //QFileInfo fileInfo(m_path);
    //return fileInfo.size();

    return m_fileInfo.fileSize;
}

QDateTime FileCheck::fileModified() const
{
    //QFileInfo fileInfo(m_path);
    //return fileInfo.lastModified();

    return m_fileInfo.lastModified;
}

bool FileCheck::fileExist() const
{
    //QFileInfo fileInfo(m_path);
    //return fileInfo.exists();

    return m_fileInfo.exists;
}

int FileCheck::machineId() const
{
    return m_machineId;
}

int FileCheck::nbOfRecords() const
{
    return m_fileInfo.nbOfRecords;
}

QDateTime FileCheck::lastInserted() const
{
    return m_fileInfo.lastInserted;
}

int FileCheck::nbOfRecordsProcessed() const
{
    return m_fileInfo.nbOfRecordsProcessed;
}

void FileCheck::timer_timeout()
{
    try
    {
        SFileCheckInfo info  = getFileInfo();

        if(!(info == m_fileInfo))
        {            
            uint64_t startMs = QDateTime::currentMSecsSinceEpoch();

            //last stored values
            info.lastInserted = m_fileInfo.lastInserted;
            info.nbOfRecordsProcessed = m_fileInfo.nbOfRecordsProcessed;

            processFile(info);
            m_fileInfo = info;

            processDatabase(info);
            m_fileInfo = info;

            uint64_t stopMs = QDateTime::currentMSecsSinceEpoch();
            qInfo().nospace() << "FileCheck(id:" << m_machineId << ") Updated"
                              << " records: " << m_fileInfo.nbOfRecords
                              << ", processed: " << m_fileInfo.nbOfRecordsProcessed
                              << ", duration: " << (stopMs-startMs) << "ms";
        }
        else
        {
            m_fileInfo.nbOfRecordsProcessed += m_fileInfo.nbOfRecords;
            m_fileInfo.nbOfRecords = 0;
        }

        m_checkTime = QDateTime::currentDateTime();

        emit fileChanged(m_index);
    }
    catch (QException& e)
    {
        qCritical().nospace() << "FileCheck(id:" << m_machineId << ") Exception: " << e.what();
    }
    catch (...)
    {
        qCritical().nospace() << "FileCheck(id:" << m_machineId << ") Unknown exception";
    }
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
    }

    return info;
}

void FileCheck::processFile(SFileCheckInfo& info)
{
    if(info.exists)
    {
        info.records.clear();

        QFileInfo fileInfoCheck(m_path + ".check");
        if(!fileInfoCheck.exists() || fileInfoCheck.lastModified() != info.lastModified)
        {
            QFile file(m_path);

            if(fileInfoCheck.exists())
                file.remove(m_path + ".check");

            bool ret = file.copy(m_path + ".check");
            if(!ret)
                qCritical().nospace().noquote() << "FileCheck(id:" << m_machineId << ") Unable to copy file: " << m_path << ".check";

            file.close();
        }

        QFile fileCheck(m_path + ".check");
        if(!fileCheck.open(QIODevice::ReadOnly))
        {
            //error
            qCritical().nospace() << "FileCheck(id:" << m_machineId << ") Unable to open file: " << m_path;
        }
        else
        {
            QTextStream in(&fileCheck);            

            int nbOfRecords = -1;
            int nbOfRecordsProcessed = 0;
            bool startRecord = false;
            QString sdt = info.lastInserted.toString("yyyyMMdd") + "," + info.lastInserted.toString("hh:mm:ss");
            while(!in.atEnd())
            {
                QString line = in.readLine();

                if(nbOfRecords<0)
                {
                    //parse header
                    info.header = line.split(",");
                    nbOfRecords = 0;
                }
                else
                {
                    //record                   
                    //QStringList recordRaw = line.split(",");
                    //QDateTime dt = QDateTime::fromString(recordRaw.at(0) + " " + recordRaw.at(1), "yyyyMMdd hh:mm:ss");

                    if(line.size()>0 && (!info.lastInserted.isValid() || startRecord))
                    {                        
                        info.records.append(line.split(","));
                        nbOfRecords++;

                        if(nbOfRecords>=cInsert_MaxNbOfRecords)
                        {
                            info.recordsWaiting = !in.atEnd();
                            break;
                        }
                    }
                    else
                    {
                        if(info.lastInserted.isValid())
                        {
                            //startRecord = (line.left(sdt.size()).contains(sdt));
                            startRecord = (0==QString::compare(line.left(sdt.size()), sdt));
                        }

                        nbOfRecordsProcessed++;
                    }
                }                                
            }

            if(info.lastInserted.isValid() && !startRecord)
            {
                qCritical() << "FileCheck(id:" << m_machineId << ") Last known data-time not found: " << info.lastInserted.toString("yyyy.MM.dd hh:mm:ss");

                //update to nearest
                in.seek(0);
                QDateTime lastInserted = info.lastInserted;
                while(!in.atEnd())
                {
                    QString line = in.readLine();
                    QStringList recordRaw = line.split(",");
                    if(recordRaw.size()>=2)
                    {
                        QDateTime dt = QDateTime::fromString(recordRaw.at(0) + " " + recordRaw.at(1), "yyyyMMdd hh:mm:ss");
                        if(dt>info.lastInserted)
                        {
                            if(lastInserted == info.lastInserted)
                                lastInserted = QDateTime();

                            break;
                        }
                        else if(dt.isValid())
                            lastInserted = dt;
                    }
                }

                if(lastInserted != info.lastInserted)
                {
                    if(lastInserted.isValid())
                        qInfo() << "FileCheck(id:" << m_machineId << ") New data-time found: " << lastInserted.toString("yyyy.MM.dd hh:mm:ss");
                    else
                        qInfo() << "FileCheck(id:" << m_machineId << ") New data-time cleared";

                    info.lastInserted = lastInserted;
                    info.recordsWaiting = true;
                }

            }

            info.nbOfRecords = nbOfRecords;
            info.nbOfRecordsProcessed = nbOfRecordsProcessed;

            fileCheck.close();
        }
    }
}

void FileCheck::processDatabase(SFileCheckInfo& info)
{
    if(info.nbOfRecords>0)
    {
        if(!m_db.isValid())
        {
            qCritical().noquote() << "FileCheck(id:" << m_machineId << ") No database driver!";
            return;
        }

        QSqlQuery query(m_db);

        //header
        int columnCnt = info.header.size();
        if(columnCnt<2)
        {
            qCritical().nospace() << "FileCheck(id:" << m_machineId << ") Header column count<2";
        }

        QStringList headerRaw = info.header;
        headerRaw.replace(0, "machine_id");
        headerRaw.replace(1, "datetime");
        QStringList header;
        foreach(auto h, headerRaw)
        {
            h = h.remove("\"");
            header.append(QString("%1").arg(h));
        }

        QStringList records;
        QDateTime lastInserted;
        foreach(auto recordRaw, info.records)
        {
            if(recordRaw.count()==columnCnt)
            {
                lastInserted = QDateTime::fromString(recordRaw.at(0) + " " + recordRaw.at(1), "yyyyMMdd hh:mm:ss");

                //record
                recordRaw.replace(0, QString::number(m_machineId));
                recordRaw.replace(1, lastInserted.toString("yyyy-MM-dd hh:mm:ss"));
                QStringList record;
                foreach(auto r, recordRaw)
                {
                    r = r.remove("\"");
                    record.append(QString("'%1'").arg(r));
                }

                records.append(QString("(%1)").arg(record.join(",")));
            }
            else if(recordRaw.join(",").size()>0)
            {
                qCritical().nospace() << "FileCheck(id:" << m_machineId << ") Data column count<" << columnCnt << ", data: " << recordRaw.join(",");
            }
        }

        if(records.size()>0)
        {
            QString squery = QString("INSERT INTO reportdataimm (%1) VALUES %2").arg(header.join(",")).arg(records.join(","));

            bool ok = query.exec(squery);
            if(!ok)
            {
                //log query
                //qInfo().noquote() << squery;

                //log error
                qCritical().noquote() << "FileCheck(id:" << m_machineId << ") Last query error: " << query.lastError().text();;
            }
            else
            {
                info.lastInserted = lastInserted;

                Settings::writeLastInsert(QString::number(m_machineId), lastInserted);
            }
        }
    }
}

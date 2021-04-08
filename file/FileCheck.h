#ifndef FILECHECK_H
#define FILECHECK_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

class FileCheck : public QObject
{
    Q_OBJECT

public:
    FileCheck(const QString& path, int index, int machineId, QObject* parent = nullptr);

    void start(int timeMs, int delayMs = -1);
    void stop();

    bool fileExist() const;
    QString fileName() const;
    QString filePath() const;
    QString fileDir() const;
    int fileSize() const;
    QDateTime fileModified() const;
    QDateTime fileCheckTime() const;
    int machineId() const;

signals:
    void fileChanged(int index);

private:

    struct SFileCheckInfo
    {
        bool        exists;
        int         fileSize;
        QDateTime   lastModified;
        int         nbOfRecords;        
        QStringList header;        

        bool operator==(const SFileCheckInfo& other) const {
            return exists == other.exists &&
                    fileSize == other.fileSize &&
                    lastModified == other.lastModified &&
                    nbOfRecords == other.nbOfRecords;
        }

        SFileCheckInfo() : exists(false), fileSize(0), nbOfRecords(0), header() {};
    };

    SFileCheckInfo getFileInfo() const;

private slots:
    void timer_timeout();
    void delay_start();

private:
    QString         m_path;
    QTimer          m_timer;
    QTimer          m_delay;
    SFileCheckInfo  m_fileInfo;
    int             m_index;
    int             m_machineId;
    QDateTime       m_checkTime;
    int             m_timeMs;
};

Q_DECLARE_METATYPE(FileCheck*)
Q_DECLARE_METATYPE(const FileCheck*)

#endif // FILECHECK_H

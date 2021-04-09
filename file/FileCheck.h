#ifndef FILECHECK_H
#define FILECHECK_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QSqlDatabase>

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
    QDateTime lastInserted() const;
    int machineId() const;
    int nbOfRecords() const;
    int nbOfRecordsProcessed() const;

    void setDatabase(QSqlDatabase database) { m_db = database; }

signals:
    void fileChanged(int index);

private:

    struct SFileCheckInfo
    {
        bool        exists;
        int         fileSize;
        QDateTime   lastModified;
        int         nbOfRecords;
        int         nbOfRecordsProcessed;
        QDateTime   lastInserted;
        bool        recordsWaiting;

        QStringList         header;
        QList<QStringList>  records;

        bool operator==(const SFileCheckInfo& other) const {
            return exists == other.exists &&
                    fileSize == other.fileSize &&
                    lastModified == other.lastModified &&
                    recordsWaiting == other.recordsWaiting;
        }

        SFileCheckInfo() : exists(false), fileSize(0), lastModified(), nbOfRecords(0), nbOfRecordsProcessed(0), lastInserted(), recordsWaiting(false), header(), records()  {};
    };

    SFileCheckInfo getFileInfo() const;
    void processFile(SFileCheckInfo& info);
    void processDatabase(SFileCheckInfo& info);

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
    QSqlDatabase    m_db;
};

Q_DECLARE_METATYPE(FileCheck*)
Q_DECLARE_METATYPE(const FileCheck*)

#endif // FILECHECK_H

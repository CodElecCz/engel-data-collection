#ifndef FILECHECK_H
#define FILECHECK_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

class FileCheck : public QObject
{
    Q_OBJECT

public:
    FileCheck(const QString& path, QObject* parent = nullptr);

    void start(int timeMs);
    void stop();

    bool fileExist() const;
    QString fileName() const;
    QString filePath() const;
    QString fileDir() const;
    QDateTime fileModified() const;

private:

    struct SFileCheckInfo
    {
        bool        valid;
        int         fileSize;
        QDateTime   lastModified;
        int         nbOfRecords;        
        QStringList header;        

        SFileCheckInfo() : valid(false), fileSize(0), nbOfRecords(0), header() {};
    };

    SFileCheckInfo getFileInfo() const;

private slots:
    void timer_timeout();

private:
    QString     m_path;
    QTimer      m_timer;
};

#endif // FILECHECK_H

#ifndef FILECHECKMODEL_H
#define FILECHECKMODEL_H

#include <QAbstractTableModel>

class FileCheck;

class FileCheckModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FileCheckModel(QObject *parent = nullptr);
    ~FileCheckModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    typedef enum _EColumn
    {
        EColumn_Name = 0,
        EColumn_Machine,
        EColumn_Exist,
        EColumn_FileSize,
        EColumn_LastCheck,
        EColumn_LastModified,
        EColumn_Dir,

        /*...*/
        EColumn_Size
    } EScenarioColumn;

    void checkStartAll(int delay = -1);
    void checkStopAll();
    void checkAdd(const QString& path, int index, int machineId);
    void checkInterval(uint32_t intervalMs);

    void clearAll();

private slots:
    void fileCheck_fileChanged(int id);

private:
    QMap<int, FileCheck*>   m_fileList;
    uint32_t                m_intervalMs;
    uint32_t                m_intervalShiftMs;
};

#endif // FILECHECKMODEL_H

#ifndef FILECHECKMODEL_H
#define FILECHECKMODEL_H

#include <QAbstractTableModel>

class FileCheck;

class FileCheckModel : public QAbstractTableModel
{
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

        /*...*/
        EColumn_Size
    } EScenarioColumn;

    void startAll();
    void stopAll();
    void clearAll();
    void fileAdd(const QString& path);

private:
    QList<FileCheck*>   m_fileList;
};

#endif // FILECHECKMODEL_H

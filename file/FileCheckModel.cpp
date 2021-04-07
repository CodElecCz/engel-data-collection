#include "FileCheckModel.h"
#include "FileCheck.h"

FileCheckModel::FileCheckModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_fileList()
{

}

FileCheckModel::~FileCheckModel()
{
    clearAll();
}

int FileCheckModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_fileList.count();
}

int FileCheckModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return EColumn_Size;
}

QVariant FileCheckModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_fileList.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        FileCheck* fileCheck = m_fileList.at(index.row());

        switch(index.column())
        {
        case EColumn_Name:
            return fileCheck->fileName();
        case EColumn_Exist:
            return fileCheck->fileExist();
        case EColumn_LastModified:
            return fileCheck->fileModified().toString("yyyy-MM-dd hh:mm:ss");
        case EColumn_Dir:
            return fileCheck->fileDir();
        }
    }
    else if (role == Qt::CheckStateRole)
    {

    }
    else if (role == Qt::UserRole)
    {
        const FileCheck* fileCheck = m_fileList.at(index.row());

        return fileCheck;
    }
    return QVariant();
}

QVariant FileCheckModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case EColumn_Name:
                return tr("Name");
            case EColumn_Exist:
                return tr("Exist");
            case EColumn_LastModified:
                return tr("LastModified");
            case EColumn_Dir:
                return tr("Directory");

            default:
                return tr("unknown");
            }
        }
        else if (orientation == Qt::Vertical)
        {
            return QString::number(section+1);
        }
    }
    return QVariant();
}

Qt::ItemFlags FileCheckModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     Qt::ItemFlags flags = QAbstractTableModel::flags(index);

     if (index.isValid())
     {
         switch(index.column())
         {
         default:
             break;
         }
     }

     return flags;
}

void FileCheckModel::startAll()
{
    foreach(auto file, m_fileList)
    {
        file->start(1000);
    }
}

void FileCheckModel::stopAll()
{
    foreach(auto file, m_fileList)
    {
        file->stop();
    }
}

void FileCheckModel::clearAll()
{
    foreach(auto file, m_fileList)
    {
        delete file;
    }
    m_fileList.clear();
}

void FileCheckModel::fileAdd(const QString& path)
{
    FileCheck* fileCheck = new FileCheck(path, this);
    m_fileList.append(fileCheck);
}

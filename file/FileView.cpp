#include "file/FileView.h"
#include "file/FileCheck.h"
#include "file/FileCheckModel.h"
#include "settings/Settings.h"

#include "ui_FileView.h"

#include <QSettings>
#include <QFile>

FileView::FileView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileView),
    m_model(new FileCheckModel(this))
{
    ui->setupUi(this);   

    ui->tableView->setModel(m_model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setCascadingSectionResizes(false);
    //ui->tableView->verticalHeader()->setVisible(false);

    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_Name, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_Machine, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_Exist, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_FileSize, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_LastModified, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_LastCheck, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_NbOfRecords, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_NbOfRecordsProcessed, QHeaderView::ResizeToContents);

    ui->tableView->horizontalHeader()->setSectionResizeMode(FileCheckModel::EColumn_Dir, QHeaderView::Stretch);

    ui->checkButton->setEnabled(false);
}

FileView::~FileView()
{
    delete m_model;
    delete ui;
}

void FileView::setView(QSqlDatabase dbs)
{
    settings(dbs);

    ui->checkButton->setEnabled(true);
}

void FileView::settings(QSqlDatabase dbs)
{    
    QFile configFile(Settings::iniFilePath());
    if(!configFile.exists())
        return;

    QSettings config(Settings::iniFilePath(), QSettings::IniFormat);
    config.setIniCodec("UTF-8");

    QString path = config.value(QString("engel_machines/path")).toString();
    QString reportData = config.value(QString("engel_reportdata/file")).toString();    

    int index = 1;
    QVariant vid;
    do
    {
        QString sid = QString("engel_machines/id%1").arg(index);

        vid = config.value(sid);

        if(vid.isValid())
        {
            QString filePath = QString("%1%2%3").arg(path).arg(vid.toString()).arg(reportData);

            m_model->checkAdd(filePath, index, vid.toInt());
            index++;
        }

    }while(vid.isValid());

    m_model->checkDatabase(dbs);

    uint32_t interval = config.value(QString("engel_machines/interval_ms")).toUInt();
    m_model->checkInterval(interval);

    bool check = config.value(QString("engel_machines/start_check")).toBool();
    uint32_t intervalShift = config.value(QString("engel_machines/interval_shift_ms")).toUInt();
    if(check)
    {
        emit statusMessage(MessageType::Enable, "File check started");

        m_model->checkStartAll(intervalShift);
        ui->checkButton->setChecked(true);      
    }
    else
    {
        ui->checkButton->setChecked(false);
    }
}

void FileView::on_checkButton_clicked(bool checked)
{
    if(checked)
    {
        emit statusMessage(MessageType::Enable, "File check started");
        m_model->checkStartAll();        
    }
    else
    {
        m_model->checkStopAll();
        emit statusMessage(MessageType::Disable, "File check stoped");
    }
}

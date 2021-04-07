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

    settings();

    ui->tableView->setModel(m_model);
}

FileView::~FileView()
{
    delete m_model;
    delete ui;
}

void FileView::settings()
{
    QFile configFile(Settings::iniFilePath());
    if(!configFile.exists())
        return;

    QSettings config(Settings::iniFilePath(), QSettings::IniFormat);
    config.setIniCodec("UTF-8");

    QString path = config.value(QString("engel_machines/path")).toString();
    QString reportData = config.value(QString("engel_reportdata/file")).toString();

    int id = 0;
    QVariant vid;
    do
    {
        QString sid = QString("engel_machines/id%1").arg(id);

        vid = config.value(sid);

        if(vid.isValid())
        {
            QString filePath = QString("%1%2%3").arg(path).arg(vid.toString()).arg(reportData);

            m_model->fileAdd(filePath);
            id++;
        }

    }while(vid.isValid());
}

void FileView::on_pushButton_clicked(bool checked)
{
    if(checked)
    {
        m_model->startAll();
    }
    else
    {
        m_model->stopAll();
    }
}

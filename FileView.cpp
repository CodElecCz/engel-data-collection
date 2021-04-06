#include "FileView.h"
#include "FileCheck.h"

#include "ui_FileView.h"

FileView::FileView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileView)
{
    ui->setupUi(this);

    FileCheck* fileCheck1 = new FileCheck("D:\\Work\\Development\\Qt\\EngelDataCollection\\MACHINES\\176798\\E63_JOBS\\ReportDataIMM.dat", this);

    m_fileList.append(fileCheck1);
}

FileView::~FileView()
{
    foreach(auto file, m_fileList)
    {
        delete file;
    }
    m_fileList.clear();

    delete ui;
}

void FileView::on_pushButton_clicked(bool checked)
{
    if(checked)
    {
        foreach(auto file, m_fileList)
        {
            file->start(1000);
        }
    }
    else
    {
        foreach(auto file, m_fileList)
        {
            file->stop();
        }
    }
}

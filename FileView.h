#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QWidget>

namespace Ui {
class FileView;
}

class FileCheck;

class FileView : public QWidget
{
    Q_OBJECT

public:
    explicit FileView(QWidget *parent = nullptr);
    ~FileView();

private slots:
    void on_pushButton_clicked(bool checked);

private:
    Ui::FileView*       ui;
    QList<FileCheck*>   m_fileList;
};

#endif // FILEVIEW_H

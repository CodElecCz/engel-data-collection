#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class FileView;
}

class FileCheckModel;

class FileView : public QWidget
{
    Q_OBJECT

public:
    explicit FileView(QWidget *parent = nullptr);
    ~FileView();

    void setView(QSqlDatabase dbs);

signals:
    void statusMessage(int type, const QString &message);

private:
    void settings(QSqlDatabase dbs);

private slots:    
    void on_checkButton_clicked(bool checked);

private:
    Ui::FileView*       ui;
    FileCheckModel*     m_model;
};

#endif // FILEVIEW_H

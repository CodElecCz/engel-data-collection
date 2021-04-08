#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QWidget>

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

private:
    void settings();

private slots:    
    void on_checkButton_clicked(bool checked);

private:
    Ui::FileView*       ui;
    FileCheckModel*     m_model;
};

#endif // FILEVIEW_H

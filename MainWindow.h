#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "support/MainWindowExt.h"
#include "CentralView.h"

#include <QMainWindow>
#include <QButtonGroup>

namespace Ui {
class MainWindow;
}

class BrowserView;
class CentralView;
class FileView;
class SettingsView;

class MainWindow : public MainWindowExt
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initializeConnection();

    void initializeMenuBar();
    void initializeToolBar();

    void setView(CentralView::EView view);

private slots:
    void toolBarButtons_buttonClicked(int);

private:
    Ui::MainWindow* ui;

    BrowserView*    m_browserView;
    FileView*       m_fileView;
    SettingsView*   m_settingsView;

    CentralView*    m_centralView;

    QButtonGroup*       m_toolBarButtons;
    CentralView::EView  m_lastView;
};

#endif // MAINWINDOW_H

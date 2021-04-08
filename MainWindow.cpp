#include "MainWindow.h"
#include "CentralView.h"
#include "browser/BrowserView.h"
#include "file/FileView.h"
#include "settings/SettingsView.h"
#include "settings/Settings.h"

#include "ui_MainWindow.h"

#include <QFileInfo>
#include <QFile>
#include <QSettings>
#include <QtSql>
#include <QToolButton>

void addConnectionsFromCommandline(const QStringList &args, BrowserView *browser);
void addConnectionsFromSettings(BrowserView *browser);

MainWindow::MainWindow(QWidget *parent) :
    MainWindowExt(parent),
    ui(new Ui::MainWindow),

    m_browserView(new BrowserView(this)),
    m_fileView(new FileView(this)),
    m_settingsView(new SettingsView(this)),

    m_centralView(new CentralView(m_browserView, m_fileView, m_settingsView, this)),

    m_toolBarButtons(new QButtonGroup(this)),
    m_lastView(CentralView::EView_none)
{
    ui->setupUi(this);

    this->setCentralWidget(m_centralView);

    initializeConnection();

    initializeMenuBar();
    initializeToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeMenuBar()
{
#if 0
    QMenu *fileMenu = ui->menubar->addMenu(QObject::tr("&File"));
    fileMenu->addAction(QObject::tr("Add &Connection..."), [&]() { m_browserView->addConnection(); });
    fileMenu->addSeparator();
    fileMenu->addAction(QObject::tr("&Quit"), []() { qApp->quit(); });

    QMenu *helpMenu = ui->menubar->addMenu(QObject::tr("&Help"));
    helpMenu->addAction(QObject::tr("About"), [&]() { m_browserView->about(); });
    //helpMenu->addAction(QObject::tr("About Qt"), []() { qApp->aboutQt(); });
#endif
    createStyleMenu(ui->menuStyle);
}

void MainWindow::initializeToolBar()
{
    //toolBar
    QToolButton* fileManagerButton = new QToolButton(ui->toolBar);
    fileManagerButton->setAccessibleName("FileManager");
    fileManagerButton->setText("File\nManager");
    fileManagerButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileManagerButton->setToolTip(tr("File Manager View"));
    fileManagerButton->setCheckable(true);
    fileManagerButton->setChecked(true);
    fileManagerButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QToolButton* dbsBrowserButton = new QToolButton(ui->toolBar);
    dbsBrowserButton->setAccessibleName("DatabaseBrowser");
    dbsBrowserButton->setText("Database\nBrowser");
    dbsBrowserButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    dbsBrowserButton->setToolTip(tr("Database Browser View"));
    dbsBrowserButton->setCheckable(true);
    dbsBrowserButton->setChecked(false);
    dbsBrowserButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QToolButton* settingsButton = new QToolButton(ui->toolBar);
    settingsButton->setAccessibleName("Settings");
    settingsButton->setText("Settings");
    settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    settingsButton->setToolTip(tr("Application settings"));
    settingsButton->setCheckable(true);
    settingsButton->setChecked(false);
    settingsButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    ui->toolBar->addWidget(fileManagerButton);
    ui->toolBar->addWidget(dbsBrowserButton);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addWidget(settingsButton);

    m_toolBarButtons->addButton(fileManagerButton, CentralView::EView_FileManager);
    m_toolBarButtons->addButton(dbsBrowserButton, CentralView::EView_DatabaseBrowser);
    m_toolBarButtons->addButton(settingsButton, CentralView::EView_Settings);
    m_toolBarButtons->setExclusive(true);

    connect(m_toolBarButtons, SIGNAL(buttonClicked(int)), this, SLOT(toolBarButtons_buttonClicked(int)));

    setView(CentralView::EView_FileManager);
}

void MainWindow::toolBarButtons_buttonClicked(int eView)
{
    CentralView::EView view = static_cast<CentralView::EView>(eView);
    setView(view);
}

void MainWindow::setView(CentralView::EView view)
{
    //close
    if(m_lastView != view)
    {
        switch(m_lastView)
        {
        case CentralView::EView_FileManager:
            break;
        case CentralView::EView_DatabaseBrowser:
            break;
        case CentralView::EView_Settings:
            break;
        case CentralView::EView_none:
            break;
        default:
            break;
        }

        m_lastView = view;
    }

    m_centralView->setView(view);

    //open
    switch(view)
    {
    case CentralView::EView_FileManager:
        break;
    case CentralView::EView_DatabaseBrowser:
        break;
    case CentralView::EView_Settings:
        break;
    case CentralView::EView_none:
        break;
    default:
        break;
    }
}

void MainWindow::initializeConnection()
{
    //addConnectionsFromCommandline(app.arguments(), &browser);
    addConnectionsFromSettings(m_browserView);

    if (QSqlDatabase::connectionNames().isEmpty())
    {
        QMetaObject::invokeMethod(m_browserView, "addConnection", Qt::QueuedConnection);
    }

    QObject::connect(m_browserView, &BrowserView::statusMessage, [this](const QString &text) {
        ui->statusbar->showMessage(text);
    });
}

void addConnectionsFromCommandline(const QStringList &args, BrowserView *browser)
{
    for (int i = 1; i < args.count(); ++i) {
        QUrl url(args.at(i), QUrl::TolerantMode);
        if (!url.isValid()) {
            qWarning("Invalid URL: %s", qPrintable(args.at(i)));
            continue;
        }
        QSqlError err = browser->addConnection(url.scheme(), url.path().mid(1), url.host(),
                                               url.userName(), url.password(), url.port(-1));
        if (err.type() != QSqlError::NoError)
            qDebug() << "Unable to open connection:" << err;
    }
}

void addConnectionsFromSettings(BrowserView *browser)
{    
    QFile configFile(Settings::iniFilePath());
    if(!configFile.exists())
        return;

    QUrl url;

    QSettings config(Settings::iniFilePath(), QSettings::IniFormat);
    config.setIniCodec("UTF-8");

    url.setScheme(config.value(QString("odbc/driver")).toString());
    url.setPath(config.value(QString("odbc/database")).toString());
    url.setHost(config.value(QString("odbc/host")).toString());
    url.setUserName(config.value(QString("odbc/user")).toString());
    url.setPassword(config.value(QString("odbc/pasword")).toString());
    url.setPort(-1);

    QSqlError err = browser->addConnection(url.scheme().toUpper(), url.path(), url.host(),
                                           url.userName(), url.password(), url.port(-1));
    if (err.type() != QSqlError::NoError)
        qDebug() << "Unable to open connection:" << err;
}

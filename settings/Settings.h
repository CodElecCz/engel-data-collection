#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>

class Settings
{
public:
    Settings();

    static inline char const       cOrganizationName[] = "CodElec";
    static inline char const       cApplicationName[] = "ENGEL DataCollection";
    static inline char const       cVersion[] = "v1.0.0";

    static QString iniFilePath();
};

#endif // SETTINGS_H

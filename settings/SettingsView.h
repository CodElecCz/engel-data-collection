#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>

namespace Ui {
class SettingsView;
}

class SettingsView : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsView(QWidget *parent = nullptr);
    ~SettingsView();

private:
    Ui::SettingsView *ui;
};

#endif // SETTINGSVIEW_H

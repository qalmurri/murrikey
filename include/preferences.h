#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

class PreferencesWindow : public QDialog {
    Q_OBJECT
public:
    PreferencesWindow();

signals:
    void configChanged();
};

#endif

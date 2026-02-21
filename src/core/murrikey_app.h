#ifndef MURRIKEY_APP_H
#define MURRIKEY_APP_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>
#include "overlay.h"
#include "preferences.h"
#include "input_manager.h"

class MurrikeyApp : public QObject {
    Q_OBJECT

public:
    explicit MurrikeyApp(QObject *parent = nullptr);
    void start();

private:
    void setupTray();
    void setupConnections();

    ScreenkeyOverlay overlay;
    PreferencesWindow prefs;
    InputManager input;
    QSystemTrayIcon* tray;
    QTimer inputTimer;
};

#endif

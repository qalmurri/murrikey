#ifndef APP_H
#define APP_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>
#include "overlay.h"
#include "preferences.h"
#include "input_manager.h"

class App : public QObject {
    Q_OBJECT

public:
    explicit App(QObject *parent = nullptr);
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

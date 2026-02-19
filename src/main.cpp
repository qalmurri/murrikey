#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QStyle>
#include <QTimer>
#include "overlay.h"
#include "preferences.h"
#include "input_manager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    ScreenkeyOverlay overlay;
    PreferencesWindow prefs;
    InputManager input;

    QObject::connect(&input, &InputManager::keyPressed, &overlay, &ScreenkeyOverlay::handleKeyPress);
    QObject::connect(&prefs, &PreferencesWindow::configChanged, &overlay, &ScreenkeyOverlay::refresh);

    QSystemTrayIcon tray(app.style()->standardIcon(QStyle::SP_ComputerIcon));
    QMenu menu;
    menu.addAction("Preferences", &prefs, &QWidget::show);
    menu.addAction("Quit", &app, &QCoreApplication::quit);
    tray.setContextMenu(&menu);
    tray.show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &input, &InputManager::check);
    timer.start(10);

    overlay.show();
    return app.exec();
}

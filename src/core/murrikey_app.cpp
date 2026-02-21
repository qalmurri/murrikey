#include "murrikey_app.h"
#include "config.h"
#include <QMenu>
#include <QStyle>
#include <QApplication>
#include <QMessageBox>

MurrikeyApp::MurrikeyApp(QObject *parent) : QObject(parent) {
    setupConnections();
    setupTray();
}

void MurrikeyApp::setupConnections() {
    connect(&input, &InputManager::keyPressed, this, [this](QString name, bool ctrl, bool shift, bool alt) {
        int bsMode = Config::instance().load("backspace_mode", 0).toInt();
        if ((name == "BackSpace" || name == "⌫") && bsMode == 1) {
            overlay.removeLastChar();
        } else {
            overlay.handleKeyPress(name, ctrl, shift, alt);
        }
    });

    connect(&prefs, &PreferencesWindow::configChanged, &overlay, &ScreenkeyOverlay::refresh);
    connect(&prefs, &PreferencesWindow::configChanged, this, [this]() {
        int mode = Config::instance().load("input_mode", 0).toInt();
        input.setMode(mode);
    });
    connect(&inputTimer, &QTimer::timeout, &input, &InputManager::check);
}

void MurrikeyApp::setupTray() {
    tray = new QSystemTrayIcon(qApp->style()->standardIcon(QStyle::SP_ComputerIcon), this);
    QMenu* menu = new QMenu();

    menu->addAction("Preferences", &prefs, &QWidget::show);
    menu->addAction("About", [this]() {
        QMessageBox::about(nullptr, "About Murrikey",
            "<h3>Murrikey v1.0</h3>"
            "<p>Developed by <b>qalmurri</b> on ThinkPad P51.</p>"
            "<p>Built with C++ and Qt6.</p>");
    });
    menu->addSeparator();
    menu->addAction("Quit", qApp, &QCoreApplication::quit);

    tray->setContextMenu(menu);
    tray->show();
}

void MurrikeyApp::start() {
    inputTimer.start(10);
    overlay.show();
}

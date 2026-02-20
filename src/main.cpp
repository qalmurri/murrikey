#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QStyle>
#include <QTimer>
#include <QMessageBox> // Tambahkan ini untuk jendela About
#include "overlay.h"
#include "preferences.h"
#include "input_manager.h"
#include "config.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    ScreenkeyOverlay overlay;
    PreferencesWindow prefs;
    InputManager input;
    QObject::connect(&input, &InputManager::keyPressed, [&](QString name, bool ctrl, bool shift, bool alt) {
        int bsMode = Config::instance().load("backspace_mode", 0).toInt();
        if (name == "BackSpace" && bsMode == 1) {
            overlay.removeLastChar();
        } else {
            overlay.handleKeyPress(name, ctrl, shift, alt);
        }
    });
    QObject::connect(&prefs, &PreferencesWindow::configChanged, &overlay, &ScreenkeyOverlay::refresh);
    QObject::connect(&prefs, &PreferencesWindow::configChanged, [&]() {
        int mode = Config::instance().load("input_mode", 0).toInt();
        input.setMode(mode);
        overlay.refresh(); // Biar tampilan ikut update
    });
    QSystemTrayIcon tray(app.style()->standardIcon(QStyle::SP_ComputerIcon));
    QMenu menu;
    menu.addAction("Preferences", &prefs, &QWidget::show);
    menu.addAction("About", []() {
        QMessageBox aboutBox;
        aboutBox.setWindowTitle("About Murrikey");
        aboutBox.setIcon(QMessageBox::Information);
        aboutBox.setTextFormat(Qt::RichText);
        aboutBox.setText(
            "<h3>Murrikey v1.0</h3>"
            "<p>A lightweight screenkey alternative for Linux (X11).</p>"
            "<p>Developed by <b>qalmurri</b> on ThinkPad P51.</p>"
            "<p>Built with C++ and Qt6.</p>"
        );
        aboutBox.exec();
    });
    menu.addSeparator(); // Garis pembatas agar lebih rapi
    menu.addAction("Quit", &app, &QCoreApplication::quit);
    tray.setContextMenu(&menu);
    tray.show();
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &input, &InputManager::check);
    timer.start(10);
    overlay.show();
    return app.exec();
}

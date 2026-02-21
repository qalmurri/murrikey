#include "overlay.h"
#include "config.h"
#include <QGuiApplication>
#include <QScreen>
#include <QCursor>

void ScreenkeyOverlay::refresh() {
    int x = Config::instance().load("x", 400).toInt();
    int y = Config::instance().load("y", 800).toInt();
    int w = Config::instance().load("width", 500).toInt();
    int h = Config::instance().load("height", 100).toInt();

    QString color = Config::instance().load("color", "#00FF00").toString();
    QString font = Config::instance().load("font_family", "Monospace").toString();
    
    this->setMinimumSize(0, 0);
    this->setMaximumSize(16384, 16384);

    this->setGeometry(x, y, w, h);
    
    label->setFixedSize(w, h);
    
    int dynamicFontSize = h * 0.5;

    label->setStyleSheet(QString(
        "font-size: %1px; font-weight: bold; color: %2; font-family: '%3'; "
        "background-color: rgba(20, 20, 20, 220); "
        "border-radius: 10px; border: 2px solid #333;"
        "padding-bottom: 5px;"
    ).arg(dynamicFontSize).arg(color).arg(font));
    
    label->setAlignment(Qt::AlignCenter);
}

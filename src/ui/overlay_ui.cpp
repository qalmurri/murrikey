#include "overlay.h"
#include "config.h"

void ScreenkeyOverlay::refresh() {
    int y = Config::instance().load("y", 800).toInt();
    QString color = Config::instance().load("color", "#00FF00").toString();
    QString font = Config::instance().load("font_family", "Monospace").toString();
    
    move(400, y);
    label->setStyleSheet(QString(
        "font-size: 36px; font-weight: bold; color: %1; font-family: '%2'; "
        "background-color: rgba(20, 20, 20, 200); padding: 20px; "
        "border-radius: 10px; border: 2px solid #333;"
    ).arg(color).arg(font));
}

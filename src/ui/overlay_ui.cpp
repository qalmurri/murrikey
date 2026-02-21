#include "overlay.h"
#include "config.h"
#include <QGuiApplication>
#include <QScreen>
#include <QCursor>

void ScreenkeyOverlay::refresh() {
    // 1. Ambil data koordinat murni dari hasil seleksi
    int x = Config::instance().load("x", 400).toInt();
    int y = Config::instance().load("y", 800).toInt();
    int w = Config::instance().load("width", 500).toInt();
    int h = Config::instance().load("height", 100).toInt();

    QString color = Config::instance().load("color", "#00FF00").toString();
    QString font = Config::instance().load("font_family", "Monospace").toString();
    
    // 2. MATIKAN Batasan Minimum (Agar bisa sekecil apapun seleksimu)
    this->setMinimumSize(0, 0);
    this->setMaximumSize(16384, 16384); // Limit maksimal X11

    // 3. Terapkan Geometri Secara Paksa
    this->setGeometry(x, y, w, h);
    
    // 4. Pastikan Label Mengikuti Ukuran Window
    label->setFixedSize(w, h); // Kunci ukuran label agar tidak memelarkan window
    
    int dynamicFontSize = h * 0.6;

    label->setStyleSheet(QString(
        "font-size: %1px; font-weight: bold; color: %2; font-family: '%3'; "
        "background-color: rgba(20, 20, 20, 220); "
        "border-radius: 10px; border: 2px solid #333;"
    ).arg(dynamicFontSize).arg(color).arg(font));
    
    label->setAlignment(Qt::AlignCenter);
}

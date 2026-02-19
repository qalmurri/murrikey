#include "overlay.h"
#include "config.h"
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QMap>

ScreenkeyOverlay::ScreenkeyOverlay() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_TranslucentBackground);
    
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    
    auto* eff = new QGraphicsOpacityEffect(this);
    label->setGraphicsEffect(eff);
    
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(label);

    anim = new QPropertyAnimation(eff, "opacity");
    anim->setDuration(500);

    refresh();
}

void ScreenkeyOverlay::refresh() {
    int y = Config::instance().load("y", 800).toInt();
    QString c = Config::instance().load("color", "#00FF00").toString();
    QString f = Config::instance().load("font_family", "Monospace").toString();
    
    move(400, y);
    label->setStyleSheet(QString("font-size: 36px; font-weight: bold; color: %1; font-family: '%2'; "
                                 "background-color: rgba(20, 20, 20, 200); padding: 20px; "
                                 "border-radius: 10px; border: 2px solid #333;").arg(c).arg(f));
}

QString ScreenkeyOverlay::formatKey(QString key) {
    static QMap<QString, QString> map = {{"space", " "}, {"Return", "⏎"}, {"BackSpace", "⌫"}, {"plus", "+"}};
    return map.value(key, key);
}

void ScreenkeyOverlay::handleKeyPress(QString name, bool ctrl, bool shift, bool alt) {
    if (name == "BackSpace") {
        if (!buffer.isEmpty()) buffer.chop(1);
    } else {
        QString fKey = formatKey(name);
        
        // Perbaikan di sini: Tambahkan QString() di awal rangkaian
        QString full = QString(ctrl ? "Ctrl+" : "") + 
                       (alt ? "Alt+" : "") + 
                       ((shift && fKey.length() > 1) ? "Shift+" : "") + 
                       fKey;

        if (ctrl || alt || fKey.length() > 1) buffer += " [" + full + "] ";
        else buffer += fKey;
    }

    if (buffer.length() > 30) buffer = buffer.right(30);
    label->setText(buffer);
    label->graphicsEffect()->setProperty("opacity", 1.0);
    
    QTimer::singleShot(3000, this, [this]() {
        anim->setStartValue(1.0); anim->setEndValue(0.0); anim->start();
        buffer = ""; 
    });
}

#include "overlay.h"
#include "config.h"
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QMap>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

ScreenkeyOverlay::ScreenkeyOverlay() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_TranslucentBackground);
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    hideTimer = new QTimer(this);
    hideTimer->setSingleShot(true);
    connect(hideTimer, &QTimer::timeout, this, [this]() {
        buffer = ""; 
        label->setText("");
    });
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
    if (name.isEmpty()) return;
    hideTimer->stop();

    if (name == "⌫") { // Bandingkan langsung dengan simbol
        buffer += " [⌫] ";
    } else {
        QString full = QString(ctrl ? "Ctrl+" : "") + (alt ? "Alt+" : "") + 
                       ((shift && name.length() > 1) ? "Shift+" : "") + name;
        
        if (ctrl || alt || name.length() > 1) buffer += " [" + full + "] ";
        else buffer += name;
    }

    if (buffer.length() > 30) buffer = buffer.right(30);
    QString displayLayout = buffer;

    // Batasi maksimal karakter, misal 25 karakter
    int maxChars = 25; 
    if (buffer.length() > maxChars) {
        // Ambil 25 karakter terakhir dan tambahkan titik-titik di depannya
        displayLayout = "..." + buffer.right(maxChars);
    }

    label->setText(displayLayout);
    this->adjustSize(); 

    int duration = Config::instance().load("hide_duration", 3000).toInt();
    hideTimer->start(duration);
}
void ScreenkeyOverlay::removeLastChar() {
    hideTimer->stop();
    if (!buffer.isEmpty()) {
        buffer.chop(1); 
        label->setText(buffer);
        this->adjustSize();
    }
    int duration = Config::instance().load("hide_duration", 3000).toInt();
    hideTimer->start(duration);
}
void ScreenkeyOverlay::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
#ifdef Q_OS_WIN
    SetWindowPos((HWND)winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#endif
}

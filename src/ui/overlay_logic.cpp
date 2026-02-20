#include "overlay.h"
#include "config.h"

// PASTIKAN ADA ScreenkeyOverlay:: DI DEPANNYA
void ScreenkeyOverlay::handleKeyPress(QString name, bool ctrl, bool shift, bool alt) {
    if (name.isEmpty()) return;
    hideTimer->stop();

    QString formatted;
    if (name == "⌫") {
        formatted = " [⌫] ";
    } else {
        QString modifiers = "";
        if (ctrl) modifiers += "Ctrl+";
        if (alt)  modifiers += "Alt+";
        if (shift && name.length() > 1) modifiers += "Shift+";

        if (!modifiers.isEmpty() || name.length() > 1) {
            formatted = " [" + modifiers + name + "] ";
        } else {
            formatted = name;
        }
    }

    updateBuffer(formatted);
    applyEllipsis();
    this->adjustSize(); 

    int duration = Config::instance().load("hide_duration", 3000).toInt();
    hideTimer->start(duration);
}

void ScreenkeyOverlay::removeLastChar() {
    hideTimer->stop();
    if (!buffer.isEmpty()) {
        buffer.chop(1);
        applyEllipsis();
        this->adjustSize();
    }
    int duration = Config::instance().load("hide_duration", 3000).toInt();
    hideTimer->start(duration);
}

void ScreenkeyOverlay::clearBuffer() {
    buffer = ""; 
    label->setText("");
}

void ScreenkeyOverlay::updateBuffer(QString text) {
    buffer += text;
    if (buffer.length() > 100) buffer = buffer.right(100);
}

void ScreenkeyOverlay::applyEllipsis() {
    int maxChars = 25;
    if (buffer.length() > maxChars) {
        label->setText("..." + buffer.right(maxChars));
    } else {
        label->setText(buffer);
    }
}

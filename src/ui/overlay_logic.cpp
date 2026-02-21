#include "overlay.h"
#include "config.h"
#include <QFontMetrics>

void ScreenkeyOverlay::handleKeyPress(QString name, bool ctrl, bool shift, bool alt) {
    if (name.isEmpty()) return;
    hideTimer->stop();

    int threshold = Config::instance().load("smart_format_threshold", 2).toInt();

    QString currentFormatted;
    if (name == "⌫") {
        currentFormatted = " [⌫] ";
    } else {
        QString modifiers = "";
        if (ctrl) modifiers += "Ctrl+";
        if (alt)  modifiers += "Alt+";
        if (shift && name.length() > 1) modifiers += "Shift+";

        if (!modifiers.isEmpty() || name.length() > 1) {
            currentFormatted = " [" + modifiers + name + "] ";
        } else {
            currentFormatted = name;
        }
    }



    if (threshold > 0 && currentFormatted == lastKey && !currentFormatted.trimmed().isEmpty()) {
        repeatCount++;
        
        if (repeatCount >= threshold) {
            // Hapus suffix lama ( x2, x3, dst)
            if (repeatCount > threshold) {
                QString prevSuffix = QString(" x%1").arg(repeatCount - 1);
                buffer.chop(prevSuffix.length());
            }
            buffer += QString(" x%1").arg(repeatCount);
        } else {
            // Jika belum mencapai threshold, tetap tambahkan tombolnya secara normal
            updateBuffer(currentFormatted);
        }
    } else {
        // Tombol baru atau fitur dimatikan
        repeatCount = 1;
        lastKey = currentFormatted;
        updateBuffer(currentFormatted);
    }

    applyEllipsis();

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
    lastKey = "";
    repeatCount = 1;
}

void ScreenkeyOverlay::updateBuffer(QString text) {
    buffer += text;
    if (buffer.length() > 100) buffer = buffer.right(100);
}

void ScreenkeyOverlay::applyEllipsis() {
    int maxWidth = this->width() - 40; // Beri margin 40px agar tidak mepet border
    QFontMetrics metrics(label->font());
    
    QString displayText = buffer;

    // Jika lebar teks di buffer lebih besar dari lebar kotak seleksi
    if (metrics.horizontalAdvance(buffer) > maxWidth) {
        // Kita potong dari kiri sampai pas masuk ke kotak dengan tambahan "..."
        QString temp = buffer;
        while (metrics.horizontalAdvance("..." + temp) > maxWidth && temp.length() > 0) {
            temp.remove(0, 1); // Buang karakter paling kiri satu per satu
        }
        displayText = "..." + temp;
    }

    label->setText(displayText);
}

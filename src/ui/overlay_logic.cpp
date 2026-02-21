#include "overlay.h"
#include "config.h"
#include <QFontMetrics>
#include <QTimer>

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
        
        // LOGIKA BARU: 
        // Shift muncul jika:
        // 1. Nama tombol lebih dari 1 karakter (F1, Enter, dll)
        // 2. ATAU ada modifier lain yang aktif (Ctrl atau Alt)
        if (shift && (name.length() > 1 || !modifiers.isEmpty())) {
            modifiers += "Shift+";
        }

        if (!modifiers.isEmpty() || name.length() > 1) {
            // Kita paksa jadi Uppercase agar konsisten: [Ctrl+Shift+T]
            currentFormatted = " [" + modifiers + name.toUpper() + "] ";
        } else {
            currentFormatted = name;
        }
    }

    if (threshold > 0 && currentFormatted == lastKey && !currentFormatted.trimmed().isEmpty()) {
        repeatCount++;
        
        if (repeatCount >= threshold) {
            // Kita hapus tampilan angka lama (misal " x2")
            if (repeatCount > threshold) {
                QString prevSuffix = QString(" x%1").arg(repeatCount - 1);
                if (buffer.endsWith(prevSuffix)) {
                    buffer.chop(prevSuffix.length());
                }
            }
            // Tambahkan angka baru yang sinkron
            buffer += QString(" x%1").arg(repeatCount);
        } else {
            updateBuffer(currentFormatted);
        }
    } else {
        repeatCount = 1;
        lastKey = currentFormatted;
        updateBuffer(currentFormatted);
    }

    QString highlightPart = (repeatCount >= threshold) ? QString(" x%1").arg(repeatCount) : currentFormatted;
    renderWithHighlight(highlightPart);
    
    hideTimer->start(Config::instance().load("hide_duration", 3000).toInt());
}

void ScreenkeyOverlay::renderWithHighlight(QString highlightPart) {
    int maxWidth = this->width() - 40; 
    QFontMetrics metrics(label->font());
    
    QString fullText = buffer;
    
    if (metrics.horizontalAdvance(fullText) > maxWidth) {
        while (metrics.horizontalAdvance("..." + fullText) > maxWidth && fullText.length() > 0) {
            fullText.remove(0, 1);
        }
        fullText = "..." + fullText;
    }

    label->setTextFormat(Qt::RichText);

    if (fullText.endsWith(highlightPart) && !highlightPart.isEmpty()) {
        QString oldPart = fullText.left(fullText.length() - highlightPart.length());
        label->setText(oldPart + "<u>" + highlightPart + "</u>");
    } else {
        label->setText(fullText);
    }
}

void ScreenkeyOverlay::updateBuffer(QString text) {
    buffer += text;
    if (buffer.length() > 100) buffer = buffer.right(100);
}

void ScreenkeyOverlay::clearBuffer() {
    buffer = ""; 
    label->clear();
    lastKey = "";
    repeatCount = 1;
}

void ScreenkeyOverlay::removeLastChar() {
    hideTimer->stop();
    if (!buffer.isEmpty()) {
        buffer.chop(1);
        renderWithHighlight("");
    }
    int duration = Config::instance().load("hide_duration", 3000).toInt();
    hideTimer->start(duration);
}

void ScreenkeyOverlay::applyEllipsis() {
    renderWithHighlight(""); 
}

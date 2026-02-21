#include "overlay.h"
#include "config.h"
#include <QFontMetrics>
#include <QTimer>

void ScreenkeyOverlay::handleKeyPress(QString name, bool ctrl, bool shift, bool alt) {
    if (name.isEmpty()) return;
    hideTimer->stop();

    int threshold = Config::instance().load("smart_format_threshold", 2).toInt();

    // 1. Formatting nama tombol
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

    // 2. Logika Smart Formatting / Aggregation
    // Bagian ini sekarang hanya mengurus "ISI" buffer.
    if (threshold > 0 && currentFormatted == lastKey && !currentFormatted.trimmed().isEmpty()) {
        repeatCount++;
        if (repeatCount >= threshold) {
            if (repeatCount > threshold) {
                QString prevSuffix = QString(" x%1").arg(repeatCount - 1);
                buffer.chop(prevSuffix.length());
            }
            buffer += QString(" x%1").arg(repeatCount);
        } else {
            updateBuffer(currentFormatted);
        }
    } else {
        repeatCount = 1;
        lastKey = currentFormatted;
        updateBuffer(currentFormatted);
    }

    // 3. Render ke UI dengan Highlight pada Key Terakhir
    // Kita ambil bagian yang baru saja ditambahkan ke buffer untuk di-highlight
    // Jika Smart Format aktif (misal x3), maka " x3" lah yang di-underline.
    QString highlightPart;
    if (repeatCount >= threshold && threshold > 0) {
        highlightPart = QString(" x%1").arg(repeatCount);
    } else {
        highlightPart = currentFormatted;
    }

    renderWithHighlight(highlightPart);

    int duration = Config::instance().load("hide_duration", 3000).toInt();
    hideTimer->start(duration);
}

void ScreenkeyOverlay::renderWithHighlight(QString highlightPart) {
    int maxWidth = this->width() - 40; 
    QFontMetrics metrics(label->font());
    
    QString fullText = buffer;
    
    // Logika Ellipsis (Clipping)
    if (metrics.horizontalAdvance(fullText) > maxWidth) {
        while (metrics.horizontalAdvance("..." + fullText) > maxWidth && fullText.length() > 0) {
            fullText.remove(0, 1);
        }
        fullText = "..." + fullText;
    }

    // Terapkan Underline hanya pada bagian akhir (key terbaru)
    label->setTextFormat(Qt::RichText);
    if (fullText.endsWith(highlightPart) && !highlightPart.isEmpty()) {
        QString oldPart = fullText.left(fullText.length() - highlightPart.length());
        label->setText(oldPart + "<u>" + highlightPart + "</u>");
    } else {
        label->setText(fullText);
    }
}

// --- Fungsi Helper Tetap Sama ---

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
        renderWithHighlight(""); // Render ulang tanpa highlight setelah dihapus
    }
    int duration = Config::instance().load("hide_duration", 3000).toInt();
    hideTimer->start(duration);
}

void ScreenkeyOverlay::applyEllipsis() {
    // Fungsi ini sekarang opsional karena logikanya sudah masuk ke renderWithHighlight
    renderWithHighlight(""); 
}

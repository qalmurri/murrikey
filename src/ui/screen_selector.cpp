#include "screen_selector.h"
#include <QMouseEvent>
#include <QShowEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>

ScreenSelector::ScreenSelector(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_X11NetWmWindowTypeToolBar); 

    // GABUNG SEMUA FLAGS DI SINI (Pakai operator OR '|')
    setWindowFlags(Qt::FramelessWindowHint | 
                   Qt::WindowStaysOnTopHint | 
                   Qt::Tool | 
                   Qt::X11BypassWindowManagerHint); 

    setCursor(Qt::CrossCursor);
    
    crosshairPen = QPen(QColor(0, 255, 0, 150), 1, Qt::DashLine);
    selectionPen = QPen(QColor(0, 255, 0), 2, Qt::SolidLine);
    infoFont = QFont("Monospace", 10, QFont::Bold);
    
    isSelecting = false;
    setMouseTracking(true); 
}

void ScreenSelector::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    // Pastikan geometri menutupi seluruh layar virtual (termasuk multi-monitor jika ada)
    setGeometry(QGuiApplication::primaryScreen()->virtualGeometry());
}

void ScreenSelector::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        origin = event->pos();
        isSelecting = true;
        update();
    }
}

void ScreenSelector::mouseMoveEvent(QMouseEvent *event) {
    mousePos = event->pos();
    // OPTIMASI: Gunakan update() yang cerdas (bisa dibatasi areanya jika mau lebih ekstrem)
    update(); 
}

void ScreenSelector::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isSelecting = false;
        
        // Ambil rect lokal dari selector
        QRect finalRect = QRect(origin, event->pos()).normalized();

        if (finalRect.width() > 10 && finalRect.height() > 10) {
            // SINKRONISASI KOORDINAT:
            // Karena kita pakai X11BypassWindowManagerHint, 
            // koordinat lokal (0,0) di window ini sudah SEJAJAR dengan (0,0) global.
            // Kita gunakan mapToGlobal untuk memastikan kompensasi multi-monitor.
            QRect globalRect = QRect(this->mapToGlobal(finalRect.topLeft()), finalRect.size());
            
            emit areaSelected(globalRect);
        }
        this->close();
    }
}

void ScreenSelector::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) this->close();
}

void ScreenSelector::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    // OPTIMASI: Matikan Antialiasing untuk crosshair agar garisnya tajam & enteng
    painter.setRenderHint(QPainter::Antialiasing, false);

    // 1. Gambar Background Dimmed
    painter.fillRect(rect(), QColor(0, 0, 0, 150));

    // 2. Crosshair (Pakai objek cache)
    painter.setPen(crosshairPen);
    painter.drawLine(0, mousePos.y(), width(), mousePos.y());
    painter.drawLine(mousePos.x(), 0, mousePos.x(), height());

    if (isSelecting) {
        QRect selectionRect = QRect(origin, mousePos).normalized();

        // 3. Lubangi Background
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.fillRect(selectionRect, Qt::transparent);
        
        // Kembalikan mode normal untuk gambar border
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        
        // 4. Border Area (Pakai objek cache)
        painter.setPen(selectionPen);
        painter.drawRect(selectionRect);

        // 5. Info Ukuran
        painter.setFont(infoFont);
        QString info = QString("%1x%2").arg(selectionRect.width()).arg(selectionRect.height());
        
        // Background teks
        QRect textRect(selectionRect.x(), selectionRect.y() - 22, 80, 20);
        painter.fillRect(textRect, QColor(0, 0, 0, 200));
        
        painter.setPen(Qt::white);
        painter.drawText(textRect, Qt::AlignCenter, info);
    }
}

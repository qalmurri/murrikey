#include "screen_selector.h"
#include <QMouseEvent>
#include <QShowEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>

ScreenSelector::ScreenSelector(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground); // WAJIB: Biar QPainter bisa bikin efek lubang

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setCursor(Qt::CrossCursor);
    
    // QRubberBand kita buang saja, gantinya pakai variabel isSelecting
    isSelecting = false;
}

void ScreenSelector::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    setGeometry(QGuiApplication::primaryScreen()->virtualGeometry());
}

void ScreenSelector::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        origin = event->pos(); // Titik awal klik
        isSelecting = true;
        update();
    }
}

void ScreenSelector::mouseMoveEvent(QMouseEvent *event) {
    mousePos = event->pos(); // Posisi kursor sekarang
    update(); // Ini yang bikin crosshair gerak real-time
}

void ScreenSelector::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isSelecting = false;
        QRect finalRect = QRect(origin, event->pos()).normalized();
        
        if (finalRect.width() > 10 && finalRect.height() > 10) {
            emit areaSelected(finalRect);
        }
        this->close();
    }
}

void ScreenSelector::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) this->close();
}

void ScreenSelector::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. Gambar Background Gelap (Dimmed) seluruh layar
    painter.fillRect(rect(), QColor(0, 0, 0, 150)); // Hitam transparan

    // 2. Gambar Crosshair (Garis Pembidik Hijau)
    painter.setPen(QPen(QColor(0, 255, 0, 150), 1, Qt::DashLine));
    painter.drawLine(0, mousePos.y(), width(), mousePos.y());
    painter.drawLine(mousePos.x(), 0, mousePos.x(), height());

    if (isSelecting) {
        // Ganti startPos jadi origin sesuai mousePressEvent
        QRect selectionRect = QRect(origin, mousePos).normalized();

        // 3. "Lubangi" Background (Clear Area)
        // Area seleksi jadi bening total
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.fillRect(selectionRect, Qt::transparent);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        // 4. Gambar Border Area Seleksi
        painter.setPen(QPen(QColor(0, 255, 0), 2, Qt::SolidLine));
        painter.drawRect(selectionRect);

        // 5. Info Ukuran (Real-time Resolution)
        QString info = QString("%1 x %2").arg(selectionRect.width()).arg(selectionRect.height());
        painter.setPen(Qt::white);
        painter.setFont(QFont("Monospace", 10, QFont::Bold));
        
        // Gambar background teks kecil agar terbaca
        QRect textRect(selectionRect.x(), selectionRect.y() - 25, 100, 20);
        painter.fillRect(textRect, QColor(0, 0, 0, 200));
        painter.drawText(textRect, Qt::AlignCenter, info);
    }
}

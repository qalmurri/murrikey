#include "screen_selector.h"
#include <QMouseEvent>
#include <QShowEvent> // Tambahkan ini!
#include <QGuiApplication>
#include <QScreen>
#include <QRubberBand>

ScreenSelector::ScreenSelector(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setWindowOpacity(0.4);
    setCursor(Qt::CrossCursor);
    
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

// Pastikan setiap kali show, dia mengambil snapshot layar terbaru
void ScreenSelector::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    // Cover seluruh area monitor yang tersedia
    setGeometry(QGuiApplication::primaryScreen()->virtualGeometry());
    // Reset posisi rubberband agar tidak nyangkut seleksi lama
    rubberBand->hide();
    rubberBand->setGeometry(0, 0, 0, 0);
}

void ScreenSelector::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        origin = event->pos();
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
    }
}

void ScreenSelector::mouseMoveEvent(QMouseEvent *event) {
    if (rubberBand->isVisible()) {
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    }
}

void ScreenSelector::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QRect finalRect = rubberBand->geometry();
        if (finalRect.width() > 10 && finalRect.height() > 10) {
            QRect globalRect = QRect(this->mapToGlobal(finalRect.topLeft()), finalRect.size());
            emit areaSelected(globalRect);
        }
        this->close(); // Ini akan memicu WA_DeleteOnClose
    }
}

void ScreenSelector::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) this->close();
}

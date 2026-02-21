#include "screen_selector.h"
#include <QMouseEvent>
#include <QShowEvent>
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

void ScreenSelector::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    setGeometry(QGuiApplication::primaryScreen()->virtualGeometry());
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
        this->close();
    }
}

void ScreenSelector::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) this->close();
}

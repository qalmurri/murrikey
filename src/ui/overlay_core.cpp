#include "overlay.h"
#include <QVBoxLayout>

ScreenkeyOverlay::ScreenkeyOverlay(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | 
                   Qt::Tool | Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_TranslucentBackground);

    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(label);

    hideTimer = new QTimer(this);
    hideTimer->setSingleShot(true);
    
    connect(hideTimer, &QTimer::timeout, this, [this]() {
        clearBuffer();
    });

    buffer.reserve(50); 
    refresh();
}

void ScreenkeyOverlay::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

#ifdef Q_OS_WIN
    SetWindowPos((HWND)winId(), HWND_TOPMOST, 0, 0, 0, 0, 
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#endif
}

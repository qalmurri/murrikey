#include "overlay.h"
#include <QVBoxLayout>

ScreenkeyOverlay::ScreenkeyOverlay(QWidget *parent) : QWidget(parent) {
    // 1. Setup Window (Frameless, Always on Top, Click-through)
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | 
                   Qt::Tool | Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_TranslucentBackground);

    // 2. Setup Components
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(label);

    // 3. Setup Timer
    hideTimer = new QTimer(this);
    hideTimer->setSingleShot(true);
    
    connect(hideTimer, &QTimer::timeout, this, [this]() {
        clearBuffer(); // Fungsi dari logic
    });

    buffer.reserve(50); 
    refresh(); // Fungsi dari UI
}

void ScreenkeyOverlay::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
#ifdef Q_OS_WIN
    SetWindowPos((HWND)winId(), HWND_TOPMOST, 0, 0, 0, 0, 
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#endif
}

#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QString>

class ScreenkeyOverlay : public QWidget {
    Q_OBJECT

public:
    explicit ScreenkeyOverlay(QWidget *parent = nullptr);

    // --- Bagian UI (overlay_ui.cpp) ---
    void refresh(); 

    // --- Bagian Logic (overlay_logic.cpp) ---
    void handleKeyPress(QString name, bool ctrl, bool shift, bool alt);
    void removeLastChar();
    void clearBuffer();
    void applyEllipsis(); // Logika titik-titik
    void updateBuffer(QString text);

protected:
    void showEvent(QShowEvent *event) override; // (overlay_core.cpp)

private:
    QLabel* label;
    QTimer* hideTimer;
    QString buffer;
};

#endif

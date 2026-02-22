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
    void refresh(); 
    void handleKeyPress(QString name, bool ctrl, bool shift, bool alt);
    void removeLastChar();
    void clearBuffer();
    void applyEllipsis();
    void updateBuffer(QString text);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void renderWithHighlight(QString highlightPart);
    QLabel* label;
    QTimer* hideTimer;
    QString buffer;
    QString lastKey;
    int repeatCount = 1;
};

#endif

#ifndef OVERLAY_H
#define OVERLAY_H
#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
class ScreenkeyOverlay : public QWidget {
    Q_OBJECT
public:
    ScreenkeyOverlay();
    void refresh();
    void removeLastChar();
public slots:
    void handleKeyPress(QString name, bool ctrl, bool shift, bool alt);
private:
    QTimer* hideTimer;
    QLabel* label;
    QString buffer;
    QString formatKey(QString key);
protected:
    void showEvent(QShowEvent *event) override;
};
#endif

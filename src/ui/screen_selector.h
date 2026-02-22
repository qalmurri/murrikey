#ifndef SCREEN_SELECTOR_H
#define SCREEN_SELECTOR_H

#include <QWidget>
#include <QPoint>
#include <QPen>
#include <QFont>

class ScreenSelector : public QWidget {
    Q_OBJECT
    
public:
    explicit ScreenSelector(QWidget *parent = nullptr);

signals:
    void areaSelected(QRect rect);

protected:
    void showEvent(QShowEvent *event) override; 
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPoint origin;
    QPoint mousePos;
    bool isSelecting = false;

    // OPTIMASI: Cache objek gambar agar tidak dibuat ulang di paintEvent
    QPen crosshairPen;
    QPen selectionPen;
    QFont infoFont;
};

#endif

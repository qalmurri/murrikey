#ifndef SCREEN_SELECTOR_H
#define SCREEN_SELECTOR_H

#include <QWidget>
#include <QRubberBand>
#include <QPoint>

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
    void paintEvent(QPaintEvent *event) override; // Ini yang bikin error "no declaration matches"

private:
    QRubberBand *rubberBand;
    QPoint origin;
    QPoint mousePos; // Ini yang tadi dibilang "not declared in this scope"
    bool isSelecting = false;
};

#endif

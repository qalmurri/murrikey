#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "screen_selector.h"
#include <QDialog>
#include <QLabel>

class PreferencesWindow : public QDialog {
    Q_OBJECT
public:
    PreferencesWindow();

signals:
    void configChanged();

private:
    QLabel* previewLabel; // Widget untuk preview
    class ScreenSelector* currentSelector = nullptr;
    void updatePreview(); // Fungsi internal buat refresh tampilan preview
};

#endif

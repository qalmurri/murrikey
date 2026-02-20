#ifndef PREFERENCES_H
#define PREFERENCES_H

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
    void updatePreview(); // Fungsi internal buat refresh tampilan preview
};

#endif

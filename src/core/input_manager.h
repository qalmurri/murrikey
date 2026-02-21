#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <QObject>
#include <QString>

typedef struct _XDisplay Display; 

class InputManager : public QObject {
    Q_OBJECT
public:
    InputManager();
    ~InputManager();
    
    // --- Bagian Utama (input_manager.cpp) ---
    void check(); 
    void setMode(int mode);

signals:
    // Pastikan ini ada dan namanya persis seperti yang dipanggil di main.cpp
    void keyPressed(QString name, bool ctrl, bool shift, bool alt);

private:
    // --- Helper Logic (input_x11.cpp) ---
    void processKeyChange(const char* keys, bool numLockOn, bool ctrl, bool shift, bool alt, bool caps);

    Display* display;
    char old_keys[32];
    int currentMode;
};

#endif

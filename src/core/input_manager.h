#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <QObject>
#include <QString>
#include "../utils/hardware_sync.h"

typedef struct _XDisplay Display; 

class InputManager : public QObject {
    Q_OBJECT
public:
    InputManager();
    ~InputManager();
    void check(); 
    void setMode(int mode);

signals:
    void keyPressed(QString name, bool ctrl, bool shift, bool alt);

private:
    void processKeyChange(const char* keys, bool numLockOn, bool ctrl, bool shift, bool alt, bool caps);
    void sendKey(int keycode, bool ctrl, bool shift, bool alt, bool caps);
    
    KeyboardConfig sysConfig;
    Display* display;
    char old_keys[32];
    int repeatTimers[256];
    int currentMode;
};

#endif

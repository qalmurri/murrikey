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
    void check();
    void setMode(int mode);
signals:
    void keyPressed(QString name, bool ctrl, bool shift, bool alt);
private:
    Display* display;
    char old_keys[32];
    int currentMode;
    bool isModifier(unsigned long s);
    QString mapToCustomDigit(unsigned long sym);
};
#endif

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <QObject>
#include <memory>
#include "input_backend.h"

class InputManager : public QObject {
    Q_OBJECT
public:
    explicit InputManager(QObject* parent = nullptr);
    ~InputManager();

    void check();
    void setMode(int mode);

signals:
    void keyPressed(QString name, bool ctrl, bool shift, bool alt);

private:
    std::unique_ptr<InputBackend> backend;
    int currentMode = 0;
};

#endif

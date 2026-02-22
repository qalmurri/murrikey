#ifndef INPUT_BACKEND_H
#define INPUT_BACKEND_H

#include <QString>

class InputBackend {
public:
    virtual ~InputBackend() = default;

    virtual void poll() = 0;

    // callback hook (disambungkan ke InputManager)
    std::function<void(
        const QString& key,
        bool ctrl,
        bool shift,
        bool alt
    )> onKey;
};

#endif

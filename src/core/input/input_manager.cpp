#include "input_manager.h"
#include "input_backend.h"
#include "config.h"
#include "input_x11.h"

InputManager::InputManager(QObject* parent)
    : QObject(parent)
{
    backend = std::make_unique<InputX11Backend>();

    backend->onKey = [this](const QString& key, bool ctrl, bool shift, bool alt) {
        emit keyPressed(key, ctrl, shift, alt);

    };
    currentMode = Config::instance().load("input_mode", 0).toInt();
}

InputManager::~InputManager() = default;

void InputManager::check() {
    if (backend)
        backend->poll();
}

void InputManager::setMode(int mode) {
    currentMode = mode;
}

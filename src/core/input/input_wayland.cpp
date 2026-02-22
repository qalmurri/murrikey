#include "input_backend.h"

class InputWaylandBackend : public InputBackend {
public:
    void poll() override {
        // TODO: wl_keyboard listener
    }
};

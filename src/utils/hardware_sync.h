#ifndef HARDWARE_SYNC_H
#define HARDWARE_SYNC_H

struct KeyboardConfig {
    int delay = 500;
    int interval = 30;
    int delayThreshold = 50;
    int rateThreshold = 3;
};

class HardwareSync {
public:
    static KeyboardConfig getKeyboardSettings(void* displayPtr, int pollingIntervalMs = 10);
};

#endif

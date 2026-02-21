#include "hardware_sync.h"
#include "../utils/config.h"

KeyboardConfig HardwareSync::getKeyboardSettings(void* displayPtr, int pollingIntervalMs) {
    KeyboardConfig config;

    // Ambil dari config, kalau tidak ada pakai angka "Lancar Jaya" kamu kemarin
    // Delay: 500ms, Interval: 30ms (untuk rate 33Hz)
    config.delay = Config::instance().load("repeat_delay", 500).toInt();
    config.interval = Config::instance().load("repeat_interval", 30).toInt();

    // Hitung threshold (asumsi polling 10ms)
    config.delayThreshold = config.delay / pollingIntervalMs;
    config.rateThreshold = config.interval / pollingIntervalMs;

    // Safety lock agar tidak nol
    if (config.delayThreshold < 1) config.delayThreshold = 50;
    if (config.rateThreshold < 1) config.rateThreshold = 3;

    return config;
}

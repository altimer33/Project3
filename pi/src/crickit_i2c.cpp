#include "crickit_i2c.hpp"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

constexpr uint8_t CRICKET_ADDRESS = 0x49;
constexpr uint8_t STATUS_BASE = 0x00;
constexpr uint8_t STATUS_SWRST = 0x7F;
constexpr uint8_t TIMER_BASE = 0x08;
constexpr uint8_t TIMER_PWM = 0x01;
constexpr uint8_t TIMER_FREQ = 0x02;

void CrickitI2cController::writeRegister(uint8_t base, uint8_t reg, const uint8_t *data, size_t len) {
    uint8_t buffer[16];
    buffer[0] = base;
    buffer[1] = reg;
    for (size_t i = 0; i < len ; i++) {
        buffer[i + 2] = data[i];
    }
    if (write(i2c_file, buffer, len) != static_cast<ssize_t>(len + 2)) {
        std::cerr << "Error writing to device" << std::endl;
    }
}

CrickitI2cController::CrickitI2cController(const char *device) : i2c_file(open(device, O_RDWR)) {
    if (i2c_file < 0 || ioctl(i2c_file, I2C_SLAVE, CRICKET_ADDRESS) < 0) {
        std::cerr << "Error opening device" << std::endl;
    }
    uint8_t reset = 0xFF;
    writeRegister(STATUS_BASE, STATUS_SWRST, &reset, 1);
    usleep(500000);
}

CrickitI2cController::~CrickitI2cController() {
    if (i2c_file >= 0) close(i2c_file);
}

void CrickitI2cController::setPWMFreq(uint8_t pin, uint16_t freq) {
    uint8_t payload[3] = {
        pin,
        static_cast<uint8_t>(freq >> 8),
        static_cast<uint8_t>(freq & 0xFF)
    };
    writeRegister(TIMER_BASE, TIMER_FREQ, payload, 3);
}

void CrickitI2cController::setPWMValue(uint8_t pin, uint16_t value) {
    uint8_t payload[3] = {
        pin,
        static_cast<uint8_t>(value >> 8),
        static_cast<uint8_t>(value & 0xFF)
    };
    writeRegister(TIMER_BASE, TIMER_PWM, payload, 3);
}



void CrickitI2cController::setServoAngle(uint8_t servoPin, int angle) {
    setPWMValue(servoPin, 50);
    uint16_t dutyCycle = 3277 + (angle * (6554 - 3277)) / 180;
    setPWMFreq(servoPin, dutyCycle);
}
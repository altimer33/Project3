#ifndef SQUIRRELSHOOTER_CRICKIT_I2C_HPP
#define SQUIRRELSHOOTER_CRICKIT_I2C_HPP

#include <cstdint>

inline constexpr uint8_t SERVO_1 = 17;
inline constexpr uint8_t SERVO_2 = 16;

class CrickitI2cController {
    const int i2c_file;

    void writeRegister(uint8_t base, uint8_t reg, const uint8_t *data, size_t len);

public:
    CrickitI2cController(const char *device = "/dev/i2c-1");

    ~CrickitI2cController();

    void setPWMFreq(uint8_t pin, uint16_t freq);

    void setPWMValue(uint8_t pin, uint16_t value);

    void setServoAngle(uint8_t servoPin, int angle);
};

#endif //SQUIRRELSHOOTER_CRICKIT_I2C_HPP
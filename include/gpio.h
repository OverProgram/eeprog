//
// Created by tomer on 28/10/2020.
//

#ifndef EEPROG_GPIO_H
#define EEPROG_GPIO_H

namespace gpio {
    // Register offsets (also from the BCM2835 datasheet)
    constexpr unsigned int SET_REGS_OFFSET = 0x07;
    constexpr unsigned int RESET_REGS_OFFSET = 0x0A;
    constexpr unsigned int VAL_REGS_OFFSET = 0x0D;

    // Base functions i=to interface GPIO device
    volatile void *map_gpio();
    void unmap_gpio(volatile void *gpio);

    enum PinMode {
        INPUT = 0b000,
        OUTPUT = 0b001
    };

    enum PinValue {
        SET = 1, RESET = 0
    };

    void set_pin_mode(int pin, PinMode mode);
    void set_pin(int pin, PinValue value);
    PinValue read_pin(int pin);
}

#endif //EEPROG_GPIO_H

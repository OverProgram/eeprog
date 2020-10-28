//
// Created by tomer on 28/10/2020.
//

#ifndef EEPROG_GPIO_H
#define EEPROG_GPIO_H

namespace gpio {
    // Base functions i=to interface GPIO device
    volatile void *map_gpio();
    void unmap_gpio(volatile void *gpio);

    enum PinMode {
        INPUT = 0b000,
        OUTPUT = 0b001
    };

    enum PinValue {
        SET, RESET
    };

    void set_pin_mode(int pin, PinMode mode);
    void set_pin(int pin, PinValue value);
    PinValue read_pin(int pin);
}

#endif //EEPROG_GPIO_H

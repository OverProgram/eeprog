//
// Created by tomer on 31/10/2020.
//

#ifndef EEPROG_EEPROM_H
#define EEPROG_EEPROM_H

#include <cstdint>
#include <vector>
#include "gpio.h"

class EEPROM {
public:
    EEPROM(int address_pin, int address_length, int data_pin, int ce_pin, int we_pin, int oe_pin);
    ~EEPROM();

    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address);

private:
    int address_pin;
    int address_length;
    int data_pin;
    int ce_pin;
    int we_pin;
    int oe_pin;
    static constexpr int data_length = 8;

    void write_address(uint16_t val);
    void write_data(uint8_t val);
    uint8_t read_data();

    void set_data(gpio::PinMode mode);
};

#endif //EEPROG_EEPROM_H

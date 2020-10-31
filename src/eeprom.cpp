//
// Created by tomer on 31/10/2020.
//
#include "eeprom.h"
#include <thread>
#include <chrono>

EEPROM::EEPROM(int address_pin, int address_length, int data_pin, int ce_pin, int we_pin, int oe_pin)
: address_pin(address_pin)
, address_length(address_length)
, data_pin(data_pin)
, ce_pin(ce_pin)
, we_pin(we_pin)
, oe_pin(oe_pin)
{
    gpio::set_pin_mode(ce_pin, gpio::OUTPUT);
    gpio::set_pin_mode(we_pin, gpio::OUTPUT);
    gpio::set_pin_mode(oe_pin, gpio::OUTPUT);
    for (int i = address_pin; i < address_pin + address_length; i++) {
        gpio::set_pin_mode(i, gpio::OUTPUT);
    }
    gpio::set_pin(ce_pin, gpio::RESET);
    gpio::set_pin(we_pin, gpio::SET);
    gpio::set_pin(oe_pin, gpio::SET);
}

void EEPROM::write(uint16_t address, uint8_t data) {
    set_data(gpio::OUTPUT);
    gpio::set_pin(oe_pin, gpio::SET);
    write_address(address);
    write_data(data);
    gpio::set_pin(we_pin, gpio::RESET);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    gpio::set_pin(we_pin, gpio::SET);
    gpio::set_pin(oe_pin, gpio::RESET);
}

uint8_t EEPROM::read(uint16_t address) {
    set_data(gpio::INPUT);
    write_address(address);
    gpio::set_pin(oe_pin, gpio::RESET);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return read_data();
}

void EEPROM::set_data(gpio::PinMode mode) {
    for (int i = data_pin; i < data_pin + data_length; i++) {
        gpio::set_pin_mode(i, mode);
    }
}

void EEPROM::write_address(uint16_t val) {
    volatile auto *gpio = (volatile unsigned *)gpio::map_gpio();
    int mask = 0;
    for (int i = 0; i < address_length; i++) {
        mask |= 1;
        mask <<= 1;
    }
    int write_mask = val << address_pin;
    *(gpio + gpio::SET_REGS_OFFSET) = write_mask & mask;
    *(gpio + gpio::RESET_REGS_OFFSET) = (~write_mask) & mask;
    gpio::unmap_gpio(gpio);
}

void EEPROM::write_data(uint8_t val) {
    volatile auto *gpio = (volatile unsigned *)gpio::map_gpio();
    int mask = 0;
    for (int i = 0; i < data_length; i++) {
        mask |= 1;
        mask <<= 1;
    }
    int write_mask = (val & mask) << data_pin;
    *(gpio + gpio::SET_REGS_OFFSET) = write_mask & mask;
    *(gpio + gpio::RESET_REGS_OFFSET) = (~write_mask) & (mask << data_pin);
    gpio::unmap_gpio(gpio);
}

uint8_t EEPROM::read_data() {
    volatile auto *gpio = (volatile unsigned *)gpio::map_gpio();
    int mask = 0;
    for (int i = 0; i < data_length; i++) {
        mask |= 1;
        mask <<= 1;
    }
    int read_mask = mask << data_pin;
    return ((*(gpio + gpio::VAL_REGS_OFFSET)) & read_mask) >> data_pin;
}

EEPROM::~EEPROM() = default;

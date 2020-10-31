//
// Created by tomer on 28/10/2020.
//
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <bcm_host.h>

#include "gpio.h"

namespace gpio {
    constexpr unsigned int PAGE_SIZE = 4096;
    constexpr unsigned int GPIO_OFFSET = 0x200000; // From the BCM2835 datasheet


    volatile void *map_gpio() {
        int mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
        if (mem_fd < 0) {
            throw std::runtime_error("Could not open /dev/gpiomem! are you part of the gpio group?");
        }

        volatile void *gpio_map = mmap(
                nullptr,
                PAGE_SIZE,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                mem_fd,
                bcm_host_get_peripheral_address() + GPIO_OFFSET
        );

        close(mem_fd);
        return gpio_map;
    }

    void unmap_gpio(volatile void *gpio) {
        munmap((void*)gpio, PAGE_SIZE);
    }

    void set_pin_mode(int pin, PinMode mode) {
        unsigned int reg_offset = pin / 10;
        unsigned int bit_offset = (pin % 10) * 3;
        volatile auto *gpio = (volatile unsigned *)map_gpio();
        volatile unsigned *reg = gpio + reg_offset;
        *reg &= ~0b111 << bit_offset;
        *reg |= mode << bit_offset;
        unmap_gpio(gpio);
    }

    void set_pin(int pin, PinValue value) {
        int reg_offset = pin / 32;
        switch (value) {
            case SET:
                reg_offset += SET_REGS_OFFSET;
                break;
            case RESET:
                reg_offset += RESET_REGS_OFFSET;
        }
        int bit_offset = pin % 32;
        volatile auto *gpio = (volatile unsigned *)map_gpio();
        volatile unsigned *reg = gpio + reg_offset;

        *reg = 1 << bit_offset;

        unmap_gpio(gpio);
    }

    PinValue read_pin(int pin) {
        int reg_offset = pin / 32;
        int bit_offset = pin % 32;
        volatile auto *gpio = (volatile unsigned *)map_gpio();
        volatile unsigned *reg = gpio + reg_offset + VAL_REGS_OFFSET;

        PinValue val = ((*reg) & (1 << bit_offset)) == 0 ? RESET : SET;

        unmap_gpio(gpio);
        return val;
    }
}

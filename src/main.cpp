#include <iostream>
#include <chrono>
#include <thread>
#include "gpio.h"

int main() {
    gpio::set_pin_mode(2, gpio::OUTPUT);

    for (int i = 0; i < 10; i++) {
        std::cout << "Turning LCD on" << std::endl;
        gpio::set_pin(2, gpio::SET);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Turning LCD off" << std::endl;
        gpio::set_pin(2, gpio::RESET);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

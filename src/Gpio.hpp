#ifndef GPIO_HPP
#define GPIO_HPP

#include <cstdint>

void gpioInit();
void activateDigit(uint8_t digit);
void showSegments(uint8_t mask);

#endif
#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>

void timerInit();
bool consumeDigitSwitchRequest();
uint8_t consumeAnimationStepRequest();
uint16_t getTrafficLightHalfPeriodCounter();
void startNoValueAnimationTimer();
void stopNoValueAnimationTimer();

#endif
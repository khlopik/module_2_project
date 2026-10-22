#include "HardwareConfig.hpp"
#include "Gpio.hpp"

constexpr uint64_t getPinBitMask(const gpio_num_t* pins, size_t count) {
    uint64_t mask = 0;
    for (size_t i = 0; i < count; ++i) {
        mask |= (1ULL << pins[i]);
    }
    return mask;
}

void gpioInit()
{
    const gpio_config_t segmentsConfig = {
        .pin_bit_mask = getPinBitMask(segmentPins, segmentPinCount),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&segmentsConfig);

    const gpio_config_t activatorsConfig = {
        .pin_bit_mask = (1ULL << firstDigitActivatorPin) | (1ULL << secondDigitActivatorPin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&activatorsConfig);

    gpio_set_level(segmentPins[0], 0);
    gpio_set_level(segmentPins[1], 1);
    gpio_set_level(segmentPins[2], 0);
    gpio_set_level(segmentPins[3], 0);
    gpio_set_level(segmentPins[4], 0);
    gpio_set_level(segmentPins[5], 0);
    gpio_set_level(segmentPins[6], 0);
    gpio_set_level(firstDigitActivatorPin, 1);
    gpio_set_level(secondDigitActivatorPin, 0);
}

void activateDigit(uint8_t digit)
{
    for (size_t i = 0; i < digitActivatorPinCount; ++i)
    {
        gpio_set_level(digitActivatorPins[i], digitActivatorInactiveLevel);
    }
    gpio_set_level(digitActivatorPins[digit], digitActivatorActiveLevel);
}

void showSegments(uint8_t mask)
{
    for (size_t i = 0; i < segmentPinCount; ++i)
    {
        const bool on = mask & (1u << i);
        gpio_set_level(segmentPins[i], on ? segmentActiveLevel : segmentInactiveLevel);
    }
}
#include "Gpio.hpp"
#include "Timer.hpp"
#include "HardwareConfig.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

uint8_t activeDigit = 0;
uint8_t digitPattern[] = {
    0b00000000,
    0b00000000,
};
uint8_t animationStepIndex = 0;

extern "C" void app_main()
{
    ESP_LOGI("Version", "%s", __cplusplus);
    gpioInit();
    activateDigit(1);
    timerInit();

    while (true)
    {

        const uint8_t animationStepsPassed = consumeAnimationStepRequest();

        if (animationStepsPassed > 0)
        {
            animationStepIndex = (animationStepIndex + animationStepsPassed) % animationStepCount[0];
            for (size_t i = 0; i < digitActivatorPinCount; ++i)
            {
                digitPattern[i] = noValueAnimationSteps[i][animationStepIndex];
            }
        }

        if (consumeDigitSwitchRequest())
        {
            ESP_LOGI("ActiveDigit", "%d", activeDigit);
            activateDigit(activeDigit);
            showSegments(digitPattern[activeDigit]);
            activeDigit = (activeDigit + 1) % digitActivatorPinCount;
        }

        vTaskDelay(pdMS_TO_TICKS(superloopYieldDelayMs));
    }
}
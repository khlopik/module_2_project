#include "HardwareConfig.hpp"
#include "Gpio.hpp"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gptimer.html#creating-and-starting-a-timer

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
static volatile bool digitSwitchPending = false;
static volatile uint8_t animationCounter = 0;
static volatile uint16_t trafficLightHalfTickCounter = 0;
static bool animationTimerRunning = false;

bool consumeDigitSwitchRequest()
{
    portENTER_CRITICAL(&timerMux);
    const bool digitSwitch = digitSwitchPending;
    digitSwitchPending = false;
    portEXIT_CRITICAL(&timerMux);

    return digitSwitch;
}

uint8_t consumeAnimationStepRequest()
{
    portENTER_CRITICAL(&timerMux);
    const uint8_t count = animationCounter;
    animationCounter = 0;
    portEXIT_CRITICAL(&timerMux);

    return count;
}

uint16_t getTrafficLightHalfPeriodCounter()
{
    return trafficLightHalfTickCounter;
}


gptimer_handle_t digitSwitchTimer;
gptimer_handle_t noValueAnimationTimer;
gptimer_handle_t trafficLightTimer;

static bool IRAM_ATTR digitSwitchTimerCallback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    portENTER_CRITICAL_ISR(&timerMux);
    digitSwitchPending = true;
    portEXIT_CRITICAL_ISR(&timerMux);
    return false;
}

static bool IRAM_ATTR noValueAnimationTimerCallback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    portENTER_CRITICAL_ISR(&timerMux);
    animationCounter = animationCounter + 1;
    portEXIT_CRITICAL_ISR(&timerMux);
    return false;
}

static bool IRAM_ATTR trafficLightTimerCallback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    portENTER_CRITICAL_ISR(&timerMux);
    trafficLightHalfTickCounter = (trafficLightHalfTickCounter + 1) % (trafficLightDelayTicks.total() * 2);
    portEXIT_CRITICAL_ISR(&timerMux);
    return false;
}

const gptimer_config_t timerConfig = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Select the default clock source
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 1 * 1000 * 1000,
    .intr_priority = 0,
    .flags = {
        .intr_shared = false,
        .allow_pd = false,
    },
};

gptimer_alarm_config_t digitSwitchTimerAlarmConfig = {
    .alarm_count = digitScanIntervalUs,
    .reload_count = 0,
    .flags = {
        .auto_reload_on_alarm = true,
    }
};

gptimer_alarm_config_t noValueAnimationTimerAlarmConfig = {
    .alarm_count = noValueAnimationPeriodMs * 1000ULL,
    .reload_count = 0,
    .flags = {
        .auto_reload_on_alarm = true,
    }
};

gptimer_alarm_config_t trafficLightTimerAlarmConfig = {
    .alarm_count = trafficLightHalfTickMs * 1000ULL,
    .reload_count = 0,
    .flags = {
        .auto_reload_on_alarm = true,
    }
};


gptimer_event_callbacks_t digitSwitchTimerCallbacks = {
    .on_alarm = digitSwitchTimerCallback,
};

gptimer_event_callbacks_t noValueAnimationTimerCallbacks = {
    .on_alarm = noValueAnimationTimerCallback,
};

gptimer_event_callbacks_t trafficLightTimerCallbacks = {
    .on_alarm = trafficLightTimerCallback,
};

void timerInit()
{
    ESP_ERROR_CHECK(gptimer_new_timer(&timerConfig, &digitSwitchTimer));
    ESP_ERROR_CHECK(gptimer_set_alarm_action(digitSwitchTimer, &digitSwitchTimerAlarmConfig));
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(digitSwitchTimer, &digitSwitchTimerCallbacks, NULL));
    ESP_ERROR_CHECK(gptimer_enable(digitSwitchTimer));

    ESP_ERROR_CHECK(gptimer_new_timer(&timerConfig, &noValueAnimationTimer));
    ESP_ERROR_CHECK(gptimer_set_alarm_action(noValueAnimationTimer, &noValueAnimationTimerAlarmConfig));
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(noValueAnimationTimer, &noValueAnimationTimerCallbacks, NULL));
    ESP_ERROR_CHECK(gptimer_enable(noValueAnimationTimer));

    ESP_ERROR_CHECK(gptimer_new_timer(&timerConfig, &trafficLightTimer));
    ESP_ERROR_CHECK(gptimer_set_alarm_action(trafficLightTimer, &trafficLightTimerAlarmConfig));
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(trafficLightTimer, &trafficLightTimerCallbacks, NULL));
    ESP_ERROR_CHECK(gptimer_enable(trafficLightTimer));

    ESP_ERROR_CHECK(gptimer_start(digitSwitchTimer));
    ESP_ERROR_CHECK(gptimer_start(trafficLightTimer));
    
}

void startNoValueAnimationTimer()
{
    if (animationTimerRunning) return;

    animationCounter = 0;
    gptimer_set_raw_count(noValueAnimationTimer, 0);
    ESP_ERROR_CHECK(gptimer_start(noValueAnimationTimer));
    animationTimerRunning = true;
}

void stopNoValueAnimationTimer()
{
    if (animationTimerRunning == false) return;

    ESP_ERROR_CHECK(gptimer_stop(noValueAnimationTimer));
    animationTimerRunning = false;
}
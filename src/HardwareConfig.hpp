#ifndef HARDWARECONFIG_HPP
#define HARDWARECONFIG_HPP

#include "driver/gpio.h"

constexpr gpio_num_t firstDigitActivatorPin = GPIO_NUM_1;
constexpr gpio_num_t secondDigitActivatorPin = GPIO_NUM_2;

struct SegmentPins
{
    gpio_num_t A;
    gpio_num_t B;
    gpio_num_t C;
    gpio_num_t D;
    gpio_num_t E;
    gpio_num_t F;
    gpio_num_t G;
    gpio_num_t dot;
};

constexpr SegmentPins segmentPin = {
    .A = GPIO_NUM_10,   // top
    .B = GPIO_NUM_11,   // top-right
    .C = GPIO_NUM_7,    // bottom-right
    .D = GPIO_NUM_5,    // bottom
    .E = GPIO_NUM_4,    // bottom-left
    .F = GPIO_NUM_9,    // top-left
    .G = GPIO_NUM_8,    // middle
    .dot = GPIO_NUM_NC, // decimal point
};

struct LedPins
{
    gpio_num_t Red;
    gpio_num_t Yellow;
    gpio_num_t Green;
};

constexpr LedPins ledPin = {
    .Red = GPIO_NUM_48,
    .Yellow = GPIO_NUM_47,
    .Green = GPIO_NUM_21,
};

enum Segment : uint8_t
{
    SegA,
    SegB,
    SegC,
    SegD,
    SegE,
    SegF,
    SegG,
    Dot,
};
constexpr uint8_t bit(Segment s) { return 1u << s; }

constexpr gpio_num_t digitActivatorPins[] = {
    GPIO_NUM_1,
    GPIO_NUM_2,
};

constexpr uint8_t digitActivatorPinCount = sizeof(digitActivatorPins) / sizeof(digitActivatorPins[0]);
constexpr int digitActivatorActiveLevel = 0;
constexpr int digitActivatorInactiveLevel = 1;

constexpr gpio_num_t segmentPins[] = {
    segmentPin.A, // 0b00000001
    segmentPin.B, // 0b00000010
    segmentPin.C, // 0b00000100
    segmentPin.D, // 0b00001000
    segmentPin.E, // 0b00010000
    segmentPin.F, // 0b00100000
    segmentPin.G, // 0b01000000
};

constexpr uint8_t segmentPinCount = sizeof(segmentPins) / sizeof(segmentPins[0]);
constexpr int segmentActiveLevel = 0;
constexpr int segmentInactiveLevel = 1;

constexpr uint32_t noValueAnimationPeriodMs = 100;
constexpr uint8_t noValueDigit1AnimationSteps[] = {
    bit(SegA) | bit(SegB),
    bit(SegB) | bit(SegG),
    bit(SegG) | bit(SegE),
    bit(SegE) | bit(SegD),
    bit(SegD) | bit(SegC),
    bit(SegC) | bit(SegG),
    bit(SegG) | bit(SegF),
    bit(SegF) | bit(SegA),
};
constexpr uint8_t noValueDigit2AnimationSteps[] = {
    bit(SegA) | bit(SegF),
    bit(SegF) | bit(SegG),
    bit(SegG) | bit(SegC),
    bit(SegC) | bit(SegD),
    bit(SegD) | bit(SegE),
    bit(SegE) | bit(SegG),
    bit(SegG) | bit(SegB),
    bit(SegB) | bit(SegA),
};

constexpr const uint8_t *noValueAnimationSteps[] = {
    noValueDigit1AnimationSteps,
    noValueDigit2AnimationSteps,
};

constexpr uint8_t digits[] = {
    bit(SegA) | bit(SegB) | bit(SegC) | bit(SegD) | bit(SegE) | bit(SegF),
    bit(SegB) | bit(SegC),
    bit(SegA) | bit(SegB) | bit(SegG) | bit(SegE) | bit(SegD),
    bit(SegA) | bit(SegB) | bit(SegC) | bit(SegD) | bit(SegG),
    bit(SegB) | bit(SegC) | bit(SegF) | bit(SegG),
    bit(SegA) | bit(SegF) | bit(SegG) | bit(SegC) | bit(SegD),
    bit(SegA) | bit(SegC) | bit(SegD) | bit(SegE) | bit(SegF) | bit(SegG),
    bit(SegA) | bit(SegB) | bit(SegC),
    bit(SegA) | bit(SegB) | bit(SegC) | bit(SegD) | bit(SegE) | bit(SegF) | bit(SegG),
    bit(SegA) | bit(SegB) | bit(SegC) | bit(SegD) | bit(SegF) | bit(SegG),
};

constexpr uint8_t animationStepCount[] = {
    sizeof(noValueDigit1AnimationSteps) / sizeof(noValueDigit1AnimationSteps[0]),
    sizeof(noValueDigit2AnimationSteps) / sizeof(noValueDigit2AnimationSteps[0]),
};

constexpr uint32_t digitScanIntervalUs = 4000;

constexpr uint32_t superloopYieldDelayMs = 10;

struct TrafficLightDelaysTicks
{
    uint32_t Red;
    uint32_t RedAndYellow;
    uint32_t Green;
    uint32_t GreenBlink;
    uint32_t Yellow;

    constexpr auto total() const
    {
        return Red + RedAndYellow + Green + GreenBlink + Yellow; 
    }
};

constexpr TrafficLightDelaysTicks trafficLightDelayTicks = {
    .Red = 10,
    .RedAndYellow = 3,
    .Green = 10,
    .GreenBlink = 3,
    .Yellow = 3,
};

constexpr uint32_t trafficLightHalfTickMs = 500;
constexpr uint8_t trafficLightMaxVisibleCounter = 99;

#endif
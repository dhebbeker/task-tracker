/**
 * \file .
 *
 * Circumvents problematic processor macros defined by Arduino framework.
 *
 * Some of these problems may not occur in all Arduino framework implementations.
 */

#pragma once
#include <Arduino.h>

#if defined(round)
#undef round //see https://github.com/esp8266/Arduino/issues/5787#issuecomment-465852231
#endif
#if defined(abs)
#undef abs // else it conflicts with std::abs
#endif

#if defined(ArduinoFake)

/* the definition in ArduinoFake is too primitive for testing */
#if defined(digitalPinToInterrupt)
#undef digitalPinToInterrupt
#define digitalPinToInterrupt(pinNumber) pinNumber
#endif

/*
 * Memory type attributes relevant for ESP32 must be empty defined for testing.
 * 
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/memory-types.html#how-to-place-code-in-iram
 */
#if !defined(ARDUINO_ISR_ATTR)
#define ARDUINO_ISR_ATTR
#endif
#if !defined(IRAM_ATTR)
#define IRAM_ATTR
#endif
#if !defined(DRAM_ATTR)
#define DRAM_ATTR
#endif

#endif /* defined(ArduinoFake) */

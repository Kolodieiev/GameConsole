#pragma once
#include <Arduino.h>
#include "Pin.h"
#include <unordered_map>

namespace meow
{

#define TOUCH true // Сенсорний пін
#define TACT false // Кнопка

    class Input
    {
    public:
        //----------------------------------------------------------------------------------------------------------------  Відредагуй це
        enum PinID : uint8_t
        {
            PIN_UP = 5,
            PIN_DOWN = 6,
            PIN_LEFT = 4,
            PIN_RIGHT = 7,

            PIN_OK = 8,
            PIN_BACK = 9
        };

#pragma region "don't touch this"
        void update();
        // Скинути значення всіх пінів
        void reset();

        inline void enablePin(PinID pin_id) { _buttons.at(pin_id)->enable(); }
        inline void disablePin(PinID pin_id) { _buttons.at(pin_id)->disable(); }
        inline bool isHolded(PinID pin_id) { return _buttons.at(pin_id)->isHolded(); }
        inline bool isPressed(PinID pin_id) { return _buttons.at(pin_id)->isPressed(); }
        inline bool isReleased(PinID pin_id) { return _buttons.at(pin_id)->isReleased(); }
        inline void lock(PinID pin_id, unsigned long lock_duration) { _buttons.at(pin_id)->lock(lock_duration); }

#pragma endregion "don't touch this"

    private:
        //---------------------------------------------------------------------------------------------------------------- Відредагуй це
        std::unordered_map<PinID, Pin *> _buttons = {
            {PIN_UP, new Pin(PIN_UP, TOUCH)},
            {PIN_DOWN, new Pin(PIN_DOWN, TOUCH)},
            {PIN_LEFT, new Pin(PIN_LEFT, TOUCH)},
            {PIN_RIGHT, new Pin(PIN_RIGHT, TOUCH)},

            {PIN_OK, new Pin(PIN_OK, TOUCH)},
            {PIN_BACK, new Pin(PIN_BACK, TOUCH)},
        };
    };

}
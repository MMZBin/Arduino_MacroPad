/*
    MIT License

    Copyright (c) 2024 MMZBin

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef MMZ_KEYBOARD_H
#define MMZ_KEYBOARD_H

#include <Arduino.h>

#include "Array.h"
#include "MatrixKeypad.h"
#include "Key.h"

#define MACRO_PAD_INIT(rowPins, colPins) \
    MacroPad_<sizeof(rowPins) / sizeof(uint8_t), sizeof(colPins) / sizeof(uint8_t)> MacroPad(rowPins, colPins);    \
    void loop() { MacroPad.update(); }       \
    void setup()

#define Do []()

using CallbackFunc = void (*)();

template<uint8_t ROWS, uint8_t COLS>
class MacroPad_ {
public:
    MacroPad_(uint8_t (&rowPins)[ROWS], uint8_t (&colPins)[COLS]) : keyboard_(rowPins, colPins) {
        numOfKeys_ = keyboard_.getNumOfKeys();

        for (uint8_t i = 0; i < numOfKeys_; i++) {
            KEYS.add(Key());
        }
    }

    void wait(uint32_t ms, CallbackFunc func) {
        callbacks_.add(LazyCallback(ms, func));
    }

    void update() {
        Array<uint32_t> isPressed = keyboard_.read();

        for (uint16_t i = 0; i < numOfKeys_; i++) {
            KEYS[i].update(isPressed[i / 32] & (1 << (i % 32)));
        }

        invoke();
    }

    Array<Key> KEYS;

private:
    struct LazyCallback {
    public:
        LazyCallback(uint32_t waitMs, CallbackFunc f) : func(f), executeTime(millis() + waitMs) {}

        CallbackFunc func;
        uint32_t executeTime;
    };

    inline void invoke() {
        uint32_t now = millis();
        uint8_t size = callbacks_.getSize();

        for (uint8_t i = 0; i < size; i++) {
            if (callbacks_[i].executeTime > now) { continue; }

            callbacks_[i].func();
            callbacks_.remove(i);

            if (i != 0) { i--; }
            size--;
        }
    }

    MatrixKeypad<ROWS, COLS> keyboard_;

    Array<LazyCallback> callbacks_;

    uint16_t numOfKeys_;
};

#endif

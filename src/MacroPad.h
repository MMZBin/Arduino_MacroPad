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

#include "MatrixKeypad.h"
#include "Key.h"
#include "Delay.h"

// Macros to simplify initialization.
#define MACRO_PAD_INIT(rowPins, colPins)                                                                       \
    MacroPad<sizeof(rowPins) / sizeof(uint8_t), sizeof(colPins) / sizeof(uint8_t)> macroPad(rowPins, colPins); \
    void loop() { macroPad.update(); }                                                                         \
    void setup()

// Macro to make it easier to see the definition of an anonymous function.
#define Do []()

template<uint8_t ROWS, uint8_t COLS>
class MacroPad {
public:
    MacroPad(uint8_t (&rowPins)[ROWS], uint8_t (&colPins)[COLS]) : KEYS{}, keyboard_(rowPins, colPins) {
        numOfKeys_ = keyboard_.getNumOfKeys();
    }

    void update() {
        static constexpr uint8_t READ_BITS_ZERO_INDEXING =  MatrixKeypad<ROWS, COLS>::READ_BITS_ZERO_INDEXING;
        uint32_t (&isPressed)[MatrixKeypad<ROWS, COLS>::KEYBOARD_SIZE] = keyboard_.read();

        for (uint16_t i = 0; i < numOfKeys_; i++) {
            KEYS[i].update(isPressed[i / READ_BITS_ZERO_INDEXING] & (1 << (i % READ_BITS_ZERO_INDEXING)));
        }

        MacroDelay::invoke();
    }

    Key KEYS[ROWS * COLS];

private:
    MatrixKeypad<ROWS, COLS> keyboard_;

    uint16_t numOfKeys_;
};

#endif

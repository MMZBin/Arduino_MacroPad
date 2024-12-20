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

#include <Arduino.h>
#include "Key.h"

/* public */

Key::Key()
 : lastTransTime_(0), countOfClick_(0), isPressBak_(false), isHandled_(false), isLongPressed_(false), hasOccurred_(0), callbacks_() {
    //コールバック配列の初期化
    for (CallbackFunc& func : callbacks_) {
        func = nullptr;
    }
}

void Key::registerMacro(const Event type, const CallbackFunc func) { callbacks_[static_cast<uint8_t>(type)] = func; }
void Key::removeMacro(const Event type) { registerMacro(type, nullptr); }
void Key::overrideMacros(const KeyAssign callbacks) { callbacks_ = callbacks; }

void Key::update(const bool isPressed) {
    hasOccurred_ = 0;

    uint32_t now = millis();

    if (now - lastTransTime_ >= debounceTime_) {
        if (isPressed) { onPress(now); }
        else { onRelease(now); }
    }

    isPressBak_ = isPressed; //前回の値を更新
}

void Key::invoke() const {
    for (uint8_t i = 0; i < NUM_OF_EVENTS; i++) {
        if ((hasOccurred(static_cast<Event>(i))) && (callbacks_[i] != nullptr)) {
            callbacks_[i]();
        }
    }
}

bool Key::hasOccurred(const Event type) const { return hasOccurred_ & (1 << static_cast<uint8_t>(type)); }

uint32_t Key::getStateDuration() const { return millis() - lastTransTime_; }
uint8_t Key::getCountOfClick() const { return countOfClick_; }

uint32_t Key::longThreshold_ = 500;
uint32_t Key::doubleThreshold_ = 200;
uint32_t Key::debounceTime_ = 20;

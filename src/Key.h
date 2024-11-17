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

#ifndef MMZ_KEY_H
#define MMZ_KEY_H

#include <functional>
#include <array>

using CallbackFunc = std::function<void()>;
using KeyAssign = std::array<CallbackFunc, 8>;

class Key {
public:
    //イベントの種類
    enum class Event : uint8_t {
        SINGLE,       //短押し            Short press
        LONG,         //長押し            Long press
        DOUBLE,       //ダブルクリック     Double click
        RISING_EDGE,  //立ち上がりエッジ   Rising edge
        FALLING_EDGE, //立ち下がりエッジ   Falling edge
        CHANGE_INPUT, //状態が変わったとき When the input changes
        PRESSED,      //押している間毎回   Every time the button is pressed
        RELEASED      //離している間毎回   Every time the button is released
    };

    void static init(const uint32_t longThreshold=500, const uint32_t doubleThreshold=100, const uint32_t debounceTime=20) {
        longThreshold_ = longThreshold;
        doubleThreshold_ = doubleThreshold;
        debounceTime_ = debounceTime;
    }

    Key();

    void registerMacro(const Event type, const CallbackFunc callback);
    void removeMacro(const Event type);
    void overrideMacros(const KeyAssign callbacks);

    void update(const bool isPressed);  //状態を更新する

    void invoke() const;

    bool hasOccurred(const Event type) const;

    uint32_t getStateDuration() const;
    uint8_t getCountOfClick() const;

    inline bool isPressed() const { return hasOccurred(Event::PRESSED); }
    inline uint32_t getPressTime() const { return (isPressed()) ? getStateDuration() : 0; }

    bool isPressBak_, isHandled_, isLongPressed_;
    uint8_t countOfClick_;

private:
    inline void onPress(const uint32_t now) {
        emit(Event::PRESSED);

        //立ち上がりエッジのときの処理
        if (!isPressBak_) { onRisingEdge(now); }

        //長押し判定の時間を過ぎたら
        if ((!isHandled_) && (now - lastTransTime_ > longThreshold_)) {
            emit(Event::LONG);
            isHandled_ = true;
            isLongPressed_ = true;
        }
    }

    inline void onRelease(const uint32_t now) {
        emit(Event::RELEASED);

        //時間を過ぎた&ダブルクリック待ち(再度押されなかったとき)
        if (now - lastTransTime_ > doubleThreshold_) {
            if (((countOfClick_ == 1)) && (!isLongPressed_)) {
                emit(Event::SINGLE);
            }
            countOfClick_ = 0;
        }

        //立ち下がりエッジのときの処理
        if (isPressBak_) { onFallingEdge(now); }

        isHandled_ = false;
    }

    inline void onRisingEdge(const uint32_t now) {
        emit(Event::RISING_EDGE);
        emit(Event::CHANGE_INPUT);

        lastTransTime_ = now; //押し始めた時間を記録

        //未処理&ダブルクリック待ちのとき
        if ((countOfClick_ == 1) && (!isHandled_)) {
            emit(Event::DOUBLE);
            isHandled_ = true;
        }

        countOfClick_++;
    }

    inline void onFallingEdge(const uint32_t now) {
        emit(Event::FALLING_EDGE);
        emit(Event::CHANGE_INPUT);

        lastTransTime_ = now; //離し始めた時間を記録
        isLongPressed_ = false;
    }

    inline void emit(const Event type) { hasOccurred_ |= (1 << static_cast<uint8_t>(type)); }

    static uint32_t longThreshold_, doubleThreshold_, debounceTime_;
    static constexpr uint8_t NUM_OF_EVENTS = 8;

    uint32_t lastTransTime_;

    byte hasOccurred_; //0番目のビットが短押し,1番目のビットが長押し...のように対応している

    KeyAssign callbacks_;
};

#endif

#ifndef MMZ_MACRO_PAD_UTIL_H
#define MMZ_MACRO_PAD_UTIL_H

#ifdef USE_KEYBOARD_H
inline void pressToKey(Key& key, uint8_t pressKey) {
    key.registerMacro(Key::Event::RISING_EDGE, [pressKey]() { Keyboard.press(pressKey); });
    key.registerMacro(Key::Event::FALLING_EDGE, [pressKey]() { Keyboard.release(pressKey); });
}
#endif

#endif

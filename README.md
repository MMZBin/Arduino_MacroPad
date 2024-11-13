The English used in this library is based on machine translation from Japanese.
[日本語のドキュメントはこちらから](/README.ja.md)

# Arduino MacroPad Library

This is a library that enables control of a macro pad (keyboard) with Arduino.

This library only handles key input processing and macro management, so to use it as an HID, you need an appropriate HID library. (e.g., [Keyboard library](https://github.com/arduino-libraries/Keyboard))

It is designed primarily for macro functions, so it does not include features like key mapping. (Most configurations can be achieved by setting up macros.)

This library is written for the Raspberry Pi Pico, but it should work on all Arduino boards.

## Features

### Macro
- `MACRO_PAD_INIT(rowPins, colPins)`
    - Specifies the row and column pins for the key matrix.
    - This macro simplifies initialization when using the microcontroller to control a single keyboard.
    - If you want to use multiple keyboards or combine them with other components, you need to initialize manually. (See [example](/example/configure_without_init_macro/configure_without_init_macro.ino))
    - Place this macro immediately before the setup() function, connecting with `{}`.
    - Example: `MACRO_PAD_INIT(rowPins, colPins) {...}`
- `Do`
    - A macro to easily define anonymous functions without arguments or return values.

### Macro-Related
- `Util.h`
    - Provides features for use with specific HID libraries.
    - Specify the library to be used before including this header.
        - `Keyboard.h` = `USE_KEYBOARD_H`
        - Example:
        ```cpp
            #define USE_KEYBOARD_H
            #include "Util.h"
        ```
        - `pressToKey(key, pressKey)`
            - For use with `Keyboard.h`
            - Allows a key to input another key while being held, similar to a standard keyboard.
            - Specify the target key (a `Key` object) as the first argument, and the desired input key as the second argument.
            - Since it uses the `RISING_EDGE` and `FALLING_EDGE` events, you’ll need to configure it yourself if you want to add different processes.
            - Example: `pressToKey(macroPad.KEYS[10], 'w');`

- `macroDelay(ms, func)`
    - An alternative to the `delay()` function intended for use within macros.
    - It waits without blocking processing, then executes the callback function specified in the second argument after a certain time.
    - Use this instead of the `delay()` function within macros unless there’s a specific reason.
    - Example: `macroDelay(1000, Do { Keyboard.print("Hello, world!"); });`

- The `MacroPad::KEYS` array contains information (`Key` objects) about each key.
- Available event types:

    - SINGLE (short press and release)
    - LONG (press and hold for a certain time)
    - DOUBLE (double press in a short time)
    - RISING_EDGE (moment the key is pressed)
    - FALLING_EDGE (moment the key is released)
    - PRESSED (continuously while pressed)
    - RELEASED (continuously while released)

- `Key::init(longThreshold, doubleThreshold, debounceTime)`
    - Specifies the time to detect a long press, the threshold for detecting double presses, and the period to ignore input to prevent chattering.

- `Key::registerMacro(type, func)`
    - A method to register a macro to a key.
    - The first argument specifies the event type, and the second argument specifies the callback function to execute at that event.
    - Example: `macroPad.KEYS[0].registerMacro(Key::Event::DOUBLE, Do { Keyboard.print("Hello, world!"); });`
- `Key::removeMacro(type)`
    - Removes the macro for a specified event.
    - Example: `macroPad.KEYS[0].removeMacro(Key::Event::DOUBLE);`
- `Key::hasOccurred(type)`
    - Checks if the specified event occurred on that key.
    - Use this when referring to other key events within a macro.
    - Example: `if (macroPad.KEYS[1].hasOccurred(Key::Event::PRESSED)) {...}`
- `Key::getStateDuration()`
    - Gets the duration for which the current state (pressed or not pressed) has been held.
- `Key::getCountOfClick()`
    - Gets the number of times the key has been double-pressed.
    - The count resets after double-clicking ends.
    - Using this method theoretically allows assigning unique events up to 255 double-clicks.

- `Key::isPressed()`
    - Checks if the key is pressed.
- `Key::getPressTime()`
    - Gets the time the key has been pressed.
    - Returns `0` if the key is not pressed.

---
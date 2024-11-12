#include <Keyboard.h>
#include "MacroPad.h"

uint8_t rowPins[] = { 0, 1, 2, 3 };
uint8_t colPins[] = { 4, 5, 6, 7 };

MacroPad_ MacroPad(rowPins, colPins);

void setup() {
    Keyboard.begin();

    MacroPad.KEYS[0].registerMacro(Key::Event::SINGLE, Do {
        Keyboard.print("Hello, world!");
    });

    MacroPad.KEYS[1].registerMacro(Key::Event::DOUBLE, Do {
        Keyboard.print("42");
        MacroPad.wait(5000, Do {
            Keyboard.print("5 seconds have passed.");
        });
    });

    MacroPad.KEYS[1].registerMacro(Key::Event::LONG, DO {
        if (MacroPad.KEYS[2].hasOccurred(Key::Event::PRESSING)) {
            Keyboard.print("The second and third keys are pressed.");
        }
    });
}

void loop() {
    MacroPad.update(); //これは定期的に実行する必要があります。
                       //This should be done regularly.
}
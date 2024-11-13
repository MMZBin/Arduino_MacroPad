#include <Keyboard.h>
#include "MacroPad.h"

uint8_t rowPins[] = { 0, 1, 2, 3 };
uint8_t colPins[] = { 4, 5, 6, 7 };

MacroPad macroPad(rowPins, colPins);

void setup() {
    Keyboard.begin();

    macroPad.KEYS[0].registerMacro(Key::Event::SINGLE, Do {
        Keyboard.print("Hello, world!");
    });

    macroPad.KEYS[1].registerMacro(Key::Event::DOUBLE, Do {
        Keyboard.print("42");
        macroDelay(2000, Do {
            Keyboard.print("2 seconds have passed.");
            macroDelay(1000, Do { Keyboard.print("3 seconds have passed."); });
        });
    });

    macroPad.KEYS[1].registerMacro(Key::Event::LONG, Do {
        if (macroPad.KEYS[2].hasOccurred(Key::Event::PRESSED)) {
            Keyboard.print("The second and third keys are pressed.");
        }
    });
}

void loop() {
    macroPad.update(); //これは定期的に実行する必要があります。
                       //This should be done regularly.
}
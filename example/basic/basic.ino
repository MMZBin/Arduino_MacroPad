#include <Keyboard.h> //HIDとして使用したい場合は、任意のHIDライブラリを使用します。
                      //If you want to use it as HID, use any HID library.
#include "MacroPad.h"

//キーマトリクスが接続されているピン
//Pins to which the key matrix is ​​connected
uint8_t rowPins[] = { 0, 1, 2, 3 };
uint8_t colPins[] = { 4, 5, 6, 7 };

/*
    構成を簡素化するためのマクロ("MacroPad_"オブジェクトを生成し、"loop()"関数と"setup()"関数を定義します。)
    - 後ろの中括弧は"setup()"関数の実装にあたります。
    - このマクロはマイコンを単一のマクロパッドの制御のために使用することを想定しているため、
      複数のマクロパッドを使用したい場合や"loop()"関数内で処理をしたい場合はこのマクロは使用できません。
*/
/*
    Macro to simplify configuration (creates a "MacroPad_" object and defines a "loop()" function and a "setup()" function).
    - The trailing braces are the implementation of the "setup()" function.
    - This macro assumes that the micro-controller is used to control a single macro pad,
      If you want to use multiple macro pads, or if you want to process them in the "loop()"" function, you cannot use this macro.
*/
MACRO_PAD_INIT(rowPins, colPins) {
    Keyboard.begin();

    //1番目のキーを短く一回押したときに"Hello, world!"を入力する
    //Press the first key once to type "Hello, world!"
    MacroPad.KEYS[0].registerMacro(Key::Event::SINGLE, Do {
        Keyboard.print("Hello, world!");
    });

    //2番目のキーをダブルクリックしたときに"42"を入力し、5000ミリ秒後に"5 seconds have passed."と入力する
    //When you double-click the second key, it will input "42", and after 5000 milliseconds it will input "5 seconds have passed."
    MacroPad.KEYS[1].registerMacro(Key::Event::DOUBLE, Do {
        Keyboard.print("42");
        MacroPad.wait(5000, Do {
            Keyboard.print("5 seconds have passed.");
        });
    });

    //2番目のキーを長押ししたときに3番目のキーが押されていれば、"The second and third keys are pressed."と入力する
    //If the third key is pressed when the second key is pressed and held, enter "The second and third keys are pressed.".
    MacroPad.KEYS[1].registerMacro(Key::Event::LONG, DO {
        //Macro.KEYS[2].isPressing()と書くことも出来ます。
        //You can also write Macro.KEYS[2].isPressing().
        if (MacroPad.KEYS[2].hasOccurred(Key::Event::PRESSING)) {
            Keyboard.print("The second and third keys are pressed.");
        }
    });
}
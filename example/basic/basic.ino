#include <Keyboard.h> // HIDとして使用したい場合は、任意のHIDライブラリを使用します。
                      // If you want to use it as HID, use any HID library.
#include <MacroPad.h>

#define USE_KEYBOARD_H // 使用するライブラリを選択(ユーティリティを使用する場合のみ必要)
                       // Select the library you want to use(Only required if you use the utility).
#include "Util.h"

/* 詳細な機能はドキュメントを参照してください。 */
/* For detailed functionality, please refer to the documentation. */

// キーマトリクスが接続されているピン
// Pins to which the key matrix is ​​connected
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

    // 1番目のキーを短く一回押したときに"Hello, world!"を入力する
    // Press the first key once to type "Hello, world!"
    macroPad.KEYS[0].registerMacro(Key::Event::SINGLE, Do {
        Keyboard.print("Hello, world!");
    });

    // 2番目のキーをダブルクリックしたときに"42"を入力し、2000ミリ秒後に"2 seconds have passed."と入力、
    // さらに1000ミリ秒後(押されてから3秒後)に"3 seconds have passed."と入力する
    // Enter “42” when you double-click the second key, and 2000 milliseconds later enter “2 seconds have passed,
    // after another 1000 milliseconds (3 seconds after being pressed), enter “3 seconds have passed.
    macroPad.KEYS[1].registerMacro(Key::Event::DOUBLE, Do {
        // 注意：キー入力処理自体が停止してしまうので、特別な理由がない限りマクロ内ではdelay()関数の代わりにMacroPad.delay()メソッドを使用してください。
        // Note: Use the MacroPad::delay() method instead of the delay() function in macros unless there is a special reason to do so, since the keystroke process itself will be stopped.

        Keyboard.print("42");
        macroDelay(2000, Do {
            Keyboard.print("2 seconds have passed.");
            macroDelay(1000, Do { Keyboard.print("3 seconds have passed."); });
        });

        // 注意：delay()メソッドのDoブロックの外に定義された処理は遅延しません。
        // Note: Processing defined outside the 'Do' block of the delay() method is not delayed.
        // Keyboard.print("Will be printed soon.")
    });

    // 2番目のキーを長押ししたときに3番目のキーが押されていれば、"The second and third keys are pressed."と入力する
    // If the third key is pressed when the second key is pressed and held, enter "The second and third keys are pressed.".
    macroPad.KEYS[1].registerMacro(Key::Event::LONG, Do {
        // Macro.KEYS[2].isPressing()と書くことも出来ます。
        // You can also write Macro.KEYS[2].isPressed().
        if (macroPad.KEYS[2].hasOccurred(Key::Event::PRESSED)) {
            Keyboard.print("The second and third keys are pressed.");
        }
    });

    // 3番目のキーをトリプルクリックしたときに"The key was triple clicked."と入力する
    // When the third key is triple clicked, type "The key was triple clicked".
    macroPad.KEYS[2].registerMacro(Key::Event::LONG, Do {
        // ボタンが連打された回数を取得し、3回であれば処理を実行する
        // 同じように255連打まで個別にイベントを設定できます。(理論上)
        // Obtain the number of times the button has been repeatedly pressed, and if it is three times, execute the process.
        // Similarly, up to 255 strokes can be set for individual events. (Theoretically)
        if (macroPad.KEYS[2].getCountOfClick() == 3) {
            Keyboard.print("The key was triple clicked.");
        }
    });

    // 4番目のキーが押されている間、'a'を入力する
    // Enter 'a' while the fourth key is pressed
    pressToKey(macroPad.KEYS[3], 'a');
}
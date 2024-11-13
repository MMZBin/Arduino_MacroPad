# Arduino MacroPad ライブラリ
[Click here to view the document in English.](/README.md)

Arduinoでマクロパッド(キーボード)の制御ができるようになるライブラリです。

このライブラリはキー入力の処理とマクロの管理を行うだけなので、HIDとして使用するには任意のHIDライブラリが必要です。(例: [Keyboardライブラリ](https://github.com/arduino-libraries/Keyboard))

マクロ機能を主に使うことを前提としているので、キーマップ機能などは搭載されていません。(マクロを組むことで大抵は実現できます。)

これはRaspberry Pi Pico向けに書かれていますが、すべてのArduinoボードで動作するはずです。

## 機能

### マクロ
- `MACRO_PAD_INIT(rowPins, colPins)`
    - キーマトリクスの行のピンと列のピンを指定します。
    - マイコンを単一のキーボードの制御に使う場合に初期化を簡略化するためのマクロです。
    - 複数のキーボードを使用したい場合や他のパーツと組み合わせる場合は自分で初期化する必要があります。([サンプル](/example/configure_without_init_macro/configure_without_init_macro.ino)を参照)
    - このマクロの直後がsetup()関数の実装にあたるので、{}を繋げる必要があります。
    - 例: `MACRO_PAD_INIT(rowPins, colPins) {...}`
- `Do`
    - 引数も戻り値もない無名関数をわかりやすく定義するためのマクロです。

### マクロ関連
- `Util.h`
    - 特定のHIDライブラリを使用する場合向けの機能を提供します。
    - インクルードする前に使用するライブラリを指定してください。
        - `Keyboard.h` = `USE_KEYBOARD_H`
        - 例:
        ```cpp
            #define USE_KEYBOARD_H
            #include "Util.h"
        ```
        - `pressToKey(key, pressKey)`
            - `Keyboard.h`用
            - 一般的なキーボードと同じように、あるキーを押している間あるキーを入力します。
            - 第一引数に対象のキー(`Key`オブジェクト)、第二引数に入力したいキーを指定してください。
            - `RISING_EDGE`イベントと`FALLING_EDGE`イベントを使用するため、別の処理を行いたい場合は自分で設定する必要があります。。
            - 例: `pressToKey(macroPad.KEYS[10], 'w');`

- `macroDelay(ms, func)`
    - マクロ内で使用することを想定した`delay()`関数の代替です。
    - 処理をブロックせずに待機し、一定時間後に第二引数に指定したコールバック関数を実行します。
    - マクロ内では特別な理由がない限り`delay()`関数の代わりにこれを使用してください。
    - 例: `macroDelay(1000, Do { Keyboard.print("Hello, world!"); });`

- `MacroPad::KEYS`配列に各キーの情報(`Key`オブジェクト)が含まれています。
- 利用できるイベントの種類は

    - SINGLE(一回短く押して離す)
    - LONG(一定時間押し続ける)
    - DOUBLE(短時間に二回続けて押す)
    - RISING_EDGE(キーが押された瞬間)
    - FALLING_EDGE(キーが離された瞬間)
    - PRESSED(押している間ずっと)
    - RELEASED(離している間ずっと)

- `Key::init(longThreshold, doubleThreshold, debounceTime)`
    - 長押しと判定する時間、連打と判定される猶予、チャタリング防止のために入力を無視する期間を指定します。

- `Key::registerMacro(type, func)`
    - そのキーにマクロを登録するためのメソッドです。
    - 第一引数でイベントのタイプを指定し、第二引数にその時に実行されるコールバック関数を指定します。
    - 例: `macroPad.KEYS[0].registerMacro(Key::Event::DOUBLE, Do { Keyboard.print("Hello, world!"); });`
- `Key::removeMacro(type)`
    - 指定したイベントのマクロを削除します。
    - 例: `macroPad.KEYS[0].removeMacro(Key::Event::DOUBLE);`
- `Key::hasOccurred(type)`
    - そのキーで指定したイベントが発生したかどうかを調べます。
    - マクロの中で他のキーのイベントを参照する際に使えます。
    - 例: `if (macroPad.KEYS[1].hasOccurred(Key::Event::PRESSED)) {...}`
- `Key::getStateDuration()`
    - 現在の状態(押されているかそうでないか)がどのぐらい前から続いているかを取得します。
- `Key::getCountOfClick()`
    - キーが何回連打されているかを取得します。
    - 連打が終わるとリセットされます。
    - このメソッドを使えば、理論上は255連打まで個別にイベントを割り当てることが出来ます。

- `Key::isPressed()`
    - そのキーが押されているかどうかを調べます。
- `Key::getPressTime()`
    - キーが押されている時間を取得します。
    - 押されていない場合は`0`を返します。


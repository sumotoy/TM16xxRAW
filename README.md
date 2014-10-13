TM16xxRAW
=========


An easy universal library for TM1638/40 chip (<b>not</b> the board usually selled on ebay).
**Warning**: Energia support just as it's basic stage and not tested. It compiles but doesn't mean that will works! So please wait I test it (and prolly fix) before use it with Energia MCU's!<br>

TM1638 it's a popular chip used on several modules (99% made in china) that you can easy find on ebay normally populated
with seven segment displays, 8 or more mono/bicolor leds and 8 switches, mostly compatible with the well know library by <b>Ricardo Batista</b> http://code.google.com/p/tm1638-library/.<br>
Apart ready-made modules, these chip are very useful if you want to build a custom interface since can drive more than 64leds individually (80 max!!!) and 24 buttons in the same time by just using 3 wire! In that case the library of Ricardo Batista is not useful since it's hardcoded to the chinese modules and expect only 7seg displays.<br>
The chip itself it's really interesting, it has internal 450Khz oscillator that refresh leds in matrix and internal KeyScan for switces and needs only some diode to ensure switches not interfere with leds.<br>
Here it come this library, you can use 24 buttons plus all the leds you wish and it uses less resources since there's not 7seg decoders, fonts, etc.<br>
Library can handle 64 (or 80) leds individually with overall brightness and max 24 buttons (in matrix configuration) with multiple keypress!<br>
In addition an extension of the library will let you use 7Seg displays (when ready).


If you buy from ebay one of the chinese assembled modules, go for <b>Ricardo Batista</b> library!<br>
If you buy a TM1638/40 chip and you want to use in your projects <b>take a look this library</b>.<br>

GOALS

  - 3 wire operation.
  - Chainable, 1 extra wire for chip.
  - Modular, add features you need only.
  - Usable in other libraries.
  - Tiny memory footprint.
  - Brightness freely usable (not only at init), 8 levels.
  - Internal led 64 bits memory map.
  - Can update 1 choosed column (8 bit) at time if needed.
  - Compatible with Arduinos, Due, Teensy3.x (with level converter)
  - Compatible with Energia IDE (stellaris, etc.) with level converter - Not Tested!

FAQ

  - Can be used as alternative to ricardo batista one? NO
  - Can be used in SPI? NO, data pin should works as IN/OUT so will interfere with other SPI devices
  - Can work at 3.3v? Yes and No. Chip works at 5V, you'll need a signal translator (with one channel bidirectional)
  - Keyboard scan interfere with leds? NO. Look at schematics (or check datasheet, need just a diode).
  - Can I mix leds, 7seg displays, matrix led, switches, etc? YES, when extra modules are ready.
  - Can I work with led memory map and send all changes one time? YES.

What is working now?

  - Library not optimized but working.<br>

Here's a schematics that works with the library:<br>
![image](http://i1189.photobucket.com/albums/z437/theamra/github/TM1638_small_schem-1.png "schem small")<br>

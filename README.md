TM16xxRAW
=========


An easy universal library for TM1638/40 chip (not the board usually selled on ebay). Useful for build easily an interface.

TM1638 it's a popular chip find on several modules (99% made in china) that you can easy find on ebay normally populated
with 8 seven segment displays, 8 mono/bicolor leds and 8 switches, mostly compatible with
the well know library maded by <b>Ricardo Batista</b> http://code.google.com/p/tm1638-library/.<br>
But these chip are very useful if you want to build a custom interface since can drive more than 64leds individually and 24 buttons in the same time by just using 3 wire! In that case the library of Ricardo Batista is not useful since it's hardcoded to the chinese modules and expect only 7seg displays.<br>
Here come this library, you can use 24 buttons and all leds as you wish and it uses less resources since there's not 7seg decoders,, fonts, etc.<br>
Library can handle 64 (even little more) leds individually with overall brightness and max 24 buttons (in matrix configuration) with multiple keypress!<br>
In addition an extension of the library will let you use 7Seg displays (when ready).


If you buy from ebay one of the chinese assembled modules, go for <b>Ricardo Batista</b> library!
If you buy a TM1638/40 chip and you want to use in your projects <b>take a look this library</b>.

GOALS

  - 3 wire operation.
  - Chainable, 1 extra wire for chip.
  - Modular, add features you need only.
  - Usable in other libraries.
  - Tiny memory footprint.
  - Brightness freely usable (not only at init).
  - Internal led 64 bits memory map.
  - Can update 1 choosed column (8 bit) at time if needed.

FAQ

  - Can be used as alternative to ricardo batista one? NO
  - Can be used in SPI? NO, data pin should works as IN/OUT so will interfere with other SPI devices
  - Can work at 3.3v? Yes and No. Chip works at 5V, you'll need a signal translator (with one channel bidirectional)
  - Keyboard scan interfere with leds? NO. Look at schematics (or check datasheet, need just a diode).
  - Can I mix leds, 7seg displays, matrix led, switches, etc? YES, when extra modules are ready.
  - Can I work with led memory map and send all changes one time? YES.

What is working now?

  - Library not optimized but working.
  - No schematics yet (but soon), if you are in the hurry take a look to the chip datasheet!

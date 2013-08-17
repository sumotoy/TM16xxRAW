TM16xxRAW
=========

CAUTION - Really early stage, just an experiment and should change a lot during development !!!!

An easy universal library for TM1638/40 chip (not the board usually selled on ebay). Useful for build easily an interface.

TM1638 it's a popular chip find on several modules (99% made in china) that you can easy find on ebay normally populated
with 8 seven segment displays, 8 mono/bicolor leds and 8 switches. Unfortunatly these modules are not all compatible with
the well know library maded by Ricardo Batista http://code.google.com/p/tm1638-library/ and his library it's totally
un'usable if chip used with normal leds and more than 8 switches, it's too much connected to these modules and 7seg displays.
I find that those chip are useful for building easy interfaces with 64 max leds and 24 buttons and buttons because you have
potentially the access to every single led (with overall brightness) internal chip key scan support multiple keypress, so I decide
to write a simpler library that let me freely design my own interface with this chip.
This library can be easily integrated with other library and has some extra modules that let you use matrix-led-array, 7seg displays,
single leds, etc. It's NOT intended to substitute the ricardo batista one that for ready made modules it's still the only one!



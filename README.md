amidino
=======

_A simplistic MIDI parser for Arduino_

amidino parses a stream of bytes and calls back registered handlers for specific MIDI
commands. Its primary goal is to be simple and reliable.

There is also a polyphonic to monophonic "the still-held last-pressed note plays now" algorithm included.

Features
--------
* supports parsing of voice and system realtime messages
* system common and system exclusive messages are _not_ supported and are correctly ignored
* running status support
* decoupled from the actual MIDI input - it is your task to provide the MIDI byte stream
* callback based
* small memory footprint

Installation
------------
Put the files into `libraries/amidino` subdirecory of your Arduino sketches directory and restart Arduino IDE.
An example sketch should be accessible from the `File/Examples/amidino` menu.

You can also generate the documentation using the [doxygen][dox] tool in the `amidino/doc` directory.

Usage
-----
See the included example sketch for basic usage. The doxygen docs should be extensive enough to guide
you through the rest.

Notes
-----
Developed using [Arduino][ard] 1.0.5.

Links
-----
[ard]: http://www.arduino.cc/

[dox]: http://www.stack.nl/~dimitri/doxygen/

Implemented while looking at http://home.roadrunner.com/~jgglatt/tech/midispec.htm

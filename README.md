# ofxAVTuringMachine

A simple Turing machine with up 8-bit state and symbol alphabet and a cyclical tape.

set `program` to a `uint8_t` array of size 2^(6*`bits`) to map (state, scanned symbol) -> (state, written symbol, jump distance)

use `audioStep()` inside of `audioOut()` to map scanned symbol to sound at audio rate

use `makePixels()` inside of `draw()` to scan the tape into an ofPixels object for drawing
#pragma once

#include "ofMain.h"

class ofxAVTuringMachine{
public:
    uint8_t *tape, *program;
    uint32_t index, bits, tape_length, jump_div;
    uint8_t state;

    bool print;

    ofxAVTuringMachine(uint32_t bits, uint32_t jump_div);
    void randomizeTape();
    void randomizeInstruction();
    void randomizeState();
    void randomizeIndex();
    void zeroTape();
    uint8_t rand_u8();
    uint8_t step();
    float audioStep();
    tuple<uint8_t, uint8_t, uint8_t> delta();
    uint32_t getAddress();
    shared_ptr<ofPixels> makePixels(int32_t shape);
};
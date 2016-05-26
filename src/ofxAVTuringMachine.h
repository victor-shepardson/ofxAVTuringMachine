#pragma once

#include "ofMain.h"

class ofxAVTuringMachine{
public:
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
    void tick();
    uint8_t tock();
    float audioTock();
    float audioStep();
    tuple<uint8_t, uint8_t, uint8_t> delta();
    uint32_t getAddress();
    uint32_t getProgramBytes();
    shared_ptr<ofPixels> makePixels(int32_t shape);
    uint8_t* getTape();
    void setTape(uint8_t*);
    uint8_t* getProgram();
    void setProgram(uint8_t*);
    void setProgram(shared_ptr<ofPixels>&);
    void setCurrentInstruction(tuple<uint8_t, uint8_t, uint8_t>);
    void audioOut(float * output, int bufferSize, int nChannels);

private:
    shared_ptr<ofPixels> program_pix;
    uint8_t *tape, *program;

    uint8_t to_write, to_state;
    uint32_t to_jump;
};
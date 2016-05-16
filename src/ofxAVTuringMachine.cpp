#include "ofxAVTuringMachine.h"

//scan tape row-wise into an ofPixels object
//aspect ratio determined in powers of 4 according to `shape`
//return wrapped in a shared_ptr
shared_ptr<ofPixels> ofxAVTuringMachine::makePixels(int32_t shape){
    int32_t tw = 1<<(bits-shape), th = 1<<(bits+shape);
    shared_ptr<ofPixels> pix(new ofPixels());
    pix->setFromExternalPixels(tape, tw, th, 3);
    return pix;
}

//simple audioOut drop-in for convenience
void ofxAVTuringMachine::audioOut(float * output, int bufferSize, int nChannels){
    for(size_t i = 0; i<bufferSize; i++){
        float v = audioStep();
        for(size_t c=0; c<nChannels; c++){
            output[nChannels*i+c] = v;
        }
    }
}

//step and convert the scanned symbol to a float on [-.5, .5]
float ofxAVTuringMachine::audioStep(){
    return float(step())/(1<<bits) - .5;
}

//step the turing machine and return new symbol
uint8_t ofxAVTuringMachine::step(){
    auto result = delta();
    state = get<0>(result);
    tape[index] = get<1>(result);
    uint32_t new_index = index + (int32_t(get<2>(result)) - int32_t(1<<bits)/2)/jump_div;
    index = (new_index + tape_length) % tape_length;
    return tape[index];
}
//read the program
tuple<uint8_t, uint8_t, uint8_t> ofxAVTuringMachine::delta(){
    if(!program){
        cout<<"ofxAVTuringMachine error: program not allocated"<<endl;
        return make_tuple(0,0,0);
    }
    uint8_t new_state, new_symbol, jump;
    uint32_t address = getAddress();
    //address %= program_length;
    new_state = program[address];
    new_symbol = program[address+1];
    jump = program[address+2];
    return make_tuple(new_state, new_symbol, jump);
}

//get address of instruction for current state
uint32_t ofxAVTuringMachine::getAddress(){
    //return (t.state + (int32_t(t.tape[t.index])<<bits))<<1;
    return 3*(tape[index] + (uint32_t(state)<<bits));
}

uint8_t ofxAVTuringMachine::rand_u8(){
    uint8_t ret = rand();
    return ret>>(8-bits);
    //return ofRandom((1<<bits) + 1);
}

uint32_t ofxAVTuringMachine::getProgramBytes(){
    return 3<<(2*bits);
}

uint8_t* ofxAVTuringMachine::getProgram(){
    return program;
}

void ofxAVTuringMachine::setProgram(uint8_t* ptr){
    program = ptr;
    program_pix = NULL;
}

void ofxAVTuringMachine::setProgram(shared_ptr<ofPixels> &pix){
    if((pix->getWidth())*(pix->getHeight())*(pix->getNumChannels())*(pix->getBytesPerChannel()) < getProgramBytes())
        cout << "ofxAVTuringMachine error: program pixels too small"<<endl;
    else{
        program_pix = pix;
        program = pix->getData();
    }
}

uint8_t* ofxAVTuringMachine::getTape(){
    return tape;
}

void ofxAVTuringMachine::setTape(uint8_t* ptr){
    tape = ptr;
}

void ofxAVTuringMachine::randomizeState(){
    state = rand_u8();
}

void ofxAVTuringMachine::randomizeIndex(){
    uint32_t r = RAND_MAX * rand() + rand();
    index = r % tape_length;
}

void ofxAVTuringMachine::randomizeTape(){
    if(print) cout<<"randomizing tape: ";
    for(uint32_t i=0; i<tape_length; i++){
        tape[i] = rand_u8();
        if(print) cout<<tape[i];
    }
    if(print) cout<<endl;
}

void ofxAVTuringMachine::randomizeInstruction(){
    uint32_t address = getAddress();
    //address %= program_length;
    program[address] = rand_u8();
    program[address+1] = rand_u8();
    program[address+2] = rand_u8();
}

void ofxAVTuringMachine::zeroTape(){
    memset((void*)(tape), 0, tape_length*sizeof(uint8_t));
}

//note that program is not allocated by constructor,
//so it can be externally set to the tapes of other turing machines, etc
//this is kludgy, better to allocate it as a shared_ptr so it can be replaced externally without leaking memory?
ofxAVTuringMachine::ofxAVTuringMachine(uint32_t b, uint32_t j){
    bits = b;
    tape_length = 3*(1<<(2*bits));
    jump_div = j;

    print = false;

    tape = (uint8_t*)calloc(tape_length,sizeof(uint8_t));
    program = NULL;
    state = 0;
    index = 0;
}

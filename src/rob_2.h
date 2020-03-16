#ifndef __ROB_2_AGENT__H
#define __ROB_2_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Rob_2Controller : public Process, public AgentInterface {

    public:
    Rob_2Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Rob_2 : public Agent {
    public:
    Rob_2(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Rob_2Controller c;
};

DECLARE_INTERFACE(Rob_2)

#endif
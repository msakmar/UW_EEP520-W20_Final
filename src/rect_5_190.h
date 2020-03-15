#ifndef __RECT_5_190_AGENT__H
#define __RECT_5_190_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Rect_5_190Controller : public Process, public AgentInterface {

    public:
    Rect_5_190Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Rect_5_190 : public Agent {
    public:
    Rect_5_190(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Rect_5_190Controller c;
};

DECLARE_INTERFACE(Rect_5_190)

#endif
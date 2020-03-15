#ifndef __RECT_5_70_AGENT__H
#define __RECT_5_70_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Rect_5_70Controller : public Process, public AgentInterface {

    public:
    Rect_5_70Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Rect_5_70 : public Agent {
    public:
    Rect_5_70(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Rect_5_70Controller c;
};

DECLARE_INTERFACE(Rect_5_70)

#endif
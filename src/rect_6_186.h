#ifndef __RECT_6_186_AGENT__H
#define __RECT_6_186_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Rect_6_186Controller : public Process, public AgentInterface {

    public:
    Rect_6_186Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Rect_6_186 : public Agent {
    public:
    Rect_6_186(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Rect_6_186Controller c;
};

DECLARE_INTERFACE(Rect_6_186)

#endif
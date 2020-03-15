#ifndef __RECT_6_126_AGENT__H
#define __RECT_6_126_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Rect_6_126Controller : public Process, public AgentInterface {

    public:
    Rect_6_126Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Rect_6_126 : public Agent {
    public:
    Rect_6_126(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Rect_6_126Controller c;
};

DECLARE_INTERFACE(Rect_6_126)

#endif
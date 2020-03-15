#ifndef __RECT_6_306_AGENT__H
#define __RECT_6_306_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Rect_6_306Controller : public Process, public AgentInterface {

    public:
    Rect_6_306Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Rect_6_306 : public Agent {
    public:
    Rect_6_306(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Rect_6_306Controller c;
};

DECLARE_INTERFACE(Rect_6_306)

#endif
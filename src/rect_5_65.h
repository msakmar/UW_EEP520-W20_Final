#ifndef __RECT_5_65_AGENT__H
#define __RECT_5_65_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Rect_5_65Controller : public Process, public AgentInterface {

    public:
    Rect_5_65Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Rect_5_65 : public Agent {
    public:
    Rect_5_65(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Rect_5_65Controller c;
};

DECLARE_INTERFACE(Rect_5_65)

#endif
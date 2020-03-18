#ifndef __PLACER_AGENT__H
#define __PLACER_AGENT__H 

#include "enviro.h"

using namespace enviro;

class placerController : public Process, public AgentInterface {

    public:
    placerController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class placer : public Agent {
    public:
    placer(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    placerController c;
};

DECLARE_INTERFACE(placer)

#endif
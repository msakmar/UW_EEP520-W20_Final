#ifndef __MAZE_2_AGENT__H
#define __MAZE_2_AGENT__H 

#include "enviro.h"

using namespace enviro;

class maze_2Controller : public Process, public AgentInterface {

    public:
    maze_2Controller() : Process(), AgentInterface() {}

    void init() {
        prevent_rotation();
    }
    void start() {}
    void update() {}
    void stop() {}

};

class maze_2 : public Agent {
    public:
    maze_2(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    maze_2Controller c;
};

DECLARE_INTERFACE(maze_2)

#endif
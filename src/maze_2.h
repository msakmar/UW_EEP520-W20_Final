#ifndef __MAZE_2_AGENT__H
#define __MAZE_2_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Maze_2Controller : public Process, public AgentInterface {

    public:
    Maze_2Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Maze_2 : public Agent {
    public:
    Maze_2(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Maze_2Controller c;
};

DECLARE_INTERFACE(Maze_2)

#endif
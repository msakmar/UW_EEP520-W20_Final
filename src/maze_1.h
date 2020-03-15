#ifndef __MAZE_1_AGENT__H
#define __MAZE_1_AGENT__H 

#include "enviro.h"

using namespace enviro;

class Maze_1Controller : public Process, public AgentInterface {

    public:
    Maze_1Controller() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class Maze_1 : public Agent {
    public:
    Maze_1(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    Maze_1Controller c;
};

DECLARE_INTERFACE(Maze_1)

#endif
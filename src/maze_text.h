#ifndef __MAZE_TEXT_AGENT__H
#define __MAZE_TEXT_AGENT__H 

#include "enviro.h"

using namespace enviro;

class MazeTextController : public Process, public AgentInterface {

    public:
    MazeTextController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {}
    void stop() {}

};

class MazeText : public Agent {
    public:
    MazeText(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    MazeTextController c;
};

DECLARE_INTERFACE(MazeText)

#endif
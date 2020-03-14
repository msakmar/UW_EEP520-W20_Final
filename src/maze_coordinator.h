#ifndef __MAZE_COORDINATOR_AGENT__H
#define __MAZE_COORDINATOR_AGENT__H 

#include "enviro.h"

using namespace enviro;

class MazeCoordinatorController : public Process, public AgentInterface {

    public:
    MazeCoordinatorController() : Process(), AgentInterface(), initial(0), maze_number(0) {}

    void init() {
        std::cout << "initial = " << initial << "\n";
        if( initial == 0 ) {
            std::cout << "Adding Maze_1\n";
            Agent& a = add_agent("Maze_1", 0, 0, 0, {{"fill","black"},{"stroke","black"}});
            initial = 1;
        }
        watch("SwitchMaze", [&](Event e) {
            std::cout << "Switching Maze\n";
            Agent& a = add_agent("Maze 2", 0, 0, 0, {{"fill","gray"},{"stroke","black"}});

            maze_number += 1;
        });
    }
    void start() {}
    void update() {}
    void stop() {}

    int initial;
    int maze_number;
};

class MazeCoordinator : public Agent {
    public:
    MazeCoordinator(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    MazeCoordinatorController c;
};

DECLARE_INTERFACE(MazeCoordinator)

#endif
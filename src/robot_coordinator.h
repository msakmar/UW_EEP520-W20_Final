#ifndef __ROBOT_COORDINATOR_AGENT__H
#define __ROBOT_COORDINATOR_AGENT__H 

#include "enviro.h"

using namespace enviro;

class robot_coordinatorController : public Process, public AgentInterface {

    public:
    robot_coordinatorController() : Process(), AgentInterface() {}

    void init() {
        std::cout << "Loaded Robot Coordinator!\n";

        watch("Add Robot", [&](Event e) {
            std::cout << "Adding Robot: " << e.value()["robot_type"] << "\n";
            std::cout << "Location: " << e.value()["location_x"] << ", " << e.value()["location_y"] << ", " << e.value()["location_theta"] << "\n";

            Agent& maze_1_agent = add_agent("Rob_2", 60, 60, -1.57, {{"fill","black"},{"stroke","black"}});
            //Agent& a = add_agent(e.value()["robot_type"], e.value()["location_x"], e.value()["location_y"], e.value()["location_theta"], {{"fill","gray"},{"stroke","black"}});
            //Agent& robot_1_agent = add_agent("robot_1", 60, 60, -1.57, {{"fill","gray"},{"stroke","black"}});
        });
    }
    void start() {}
    void update() {}
    void stop() {}

};

class robot_coordinator : public Agent {
    public:
    robot_coordinator(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    robot_coordinatorController c;
};

DECLARE_INTERFACE(robot_coordinator)

#endif
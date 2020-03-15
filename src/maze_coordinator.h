#ifndef __MAZE_COORDINATOR_AGENT__H
#define __MAZE_COORDINATOR_AGENT__H 

#include "enviro.h"
#include <vector>
#include <math.h>

using namespace enviro;

class MazeCoordinatorController : public Process, public AgentInterface {

    public:
    MazeCoordinatorController() : Process(), AgentInterface(), initial(0), maze_number(0), counter(0), current_maze_agents_ids() {}

    void init() {
        std::cout << "initial = " << initial << "\n";
        if( initial == 0 ) {
            std::cout << "Adding Maze 1\n";
            current_maze_agents_ids = build_maze_1();
            initial = 1;
            maze_number = 1;
        }

        /*
        *  Remove existing maze agents, stored in a current_maze_agents_ids vector
        *  Clear the current_maze_agents_ids vector
        *  Build new maze using a method
        *  Save new maze agents in the current_maze_agents vector
        */
        watch("SwitchMaze", [&](Event e) {
            std::cout << "Switching Maze\n";
            for( auto it = current_maze_agents_ids.begin(); it < current_maze_agents_ids.end(); it++ ) {
                std::cout << "Testing if Agent, " << (*it) << " exists." << "\n";
                if( agent_exists( (*it) ) ) {
                    std::cout << "Removing Agent: " << (*it) << "\n";
                    remove_agent( (*it) );
                }
            }

            //Clear the maze agent ids vector
            current_maze_agents_ids.clear();

            //Determine which maze to create next
            switch ( maze_number ) {
            case 1:
                std::cout << "Adding Maze 2\n";
                current_maze_agents_ids = build_maze_2();
                maze_number = 2;
                break;
            case 2:
                std::cout << "Adding Maze 3\n";
                current_maze_agents_ids = build_maze_3();
                maze_number = 3;
                break;
            case 3:
                std::cout << "Adding Maze 4\n";
                current_maze_agents_ids = build_maze_4();
                maze_number = 4;
                break;
            case 4:
                std::cout << "Adding Maze 1\n";
                current_maze_agents_ids = build_maze_1();
                maze_number = 1;
                break;
            default:
                // leave empty
                std::cout << "Default maze_numer switch statement.\n";
                std::cout << "Adding Maze 1\n";
                current_maze_agents_ids = build_maze_1();
            }
        });
    }

    void start() {}

    void update() {
        if ( counter++ > 100 ) {
            std::cout << "Emit event to switch Maze\n";
            emit( Event( "SwitchMaze" ) );
            counter = 0;
            initial = 0;
        }
    }
    void stop() {}

    vector<int> build_maze_original_messed_up() {
        vector<int> maze_agent_ids;
        Agent& maze_1_agent = add_agent("Maze_1", 0, 0, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Maze_1: " << maze_1_agent.get_id() << "\n";
        maze_agent_ids.push_back(maze_1_agent.get_id());
        return maze_agent_ids;
    }

    vector<int> build_maze_attempt_with_shape() {
        vector<int> maze_agent_ids;
        Agent& maze_2_agent = add_agent("Maze_2", 0, 0, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Maze_2: " << maze_2_agent.get_id() << "\n";
        maze_agent_ids.push_back(maze_2_agent.get_id());
        return maze_agent_ids;
    }

    vector<int> build_maze_1() {
        vector<int> agent_ids;
        Agent& Rect186by6one = add_agent("Rect_6_186", -90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect186by6one: " << Rect186by6one.get_id() << "\n";
        agent_ids.push_back(Rect186by6one.get_id());

        Agent& Rect186by6two = add_agent("Rect_6_186", 90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect186by6two: " << Rect186by6two.get_id() << "\n";
        agent_ids.push_back(Rect186by6two.get_id());

        Agent& Rect126by6one = add_agent("Rect_6_126", 30, 30, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect126by6one: " << Rect126by6one.get_id() << "\n";
        agent_ids.push_back(Rect126by6one.get_id());

        Agent& Rect6by186two = add_agent("Rect_6_186", 0, -90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by186two: " << Rect6by186two.get_id() << "\n";
        agent_ids.push_back(Rect6by186two.get_id());

        Agent& Rect6by126one = add_agent("Rect_6_126", 30, 90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by126one: " << Rect6by126one.get_id() << "\n";
        agent_ids.push_back(Rect6by126one.get_id());

        Agent& Rect6by66one = add_agent("Rect_6_66", -60, 30, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66one: " << Rect6by66one.get_id() << "\n";
        agent_ids.push_back(Rect6by66one.get_id());

        Agent& Rect6by66two = add_agent("Rect_6_66", 0, -30, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66two: " << Rect6by66two.get_id() << "\n";
        agent_ids.push_back(Rect6by66two.get_id());
        
        return agent_ids;
    }

    vector<int> build_maze_2() {
        vector<int> agent_ids;
        Agent& Rect186by6one = add_agent("Rect_6_186", -90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect186by6one: " << Rect186by6one.get_id() << "\n";
        agent_ids.push_back(Rect186by6one.get_id());

        Agent& Rect186by6two = add_agent("Rect_6_186", 90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect186by6two: " << Rect186by6two.get_id() << "\n";
        agent_ids.push_back(Rect186by6two.get_id());

        Agent& Rect126by6one = add_agent("Rect_6_126", -30, 30, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect126by6one: " << Rect126by6one.get_id() << "\n";
        agent_ids.push_back(Rect126by6one.get_id());

        Agent& Rect66by6one = add_agent("Rect_6_66", 30, -60, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect66by6one: " << Rect66by6one.get_id() << "\n";
        agent_ids.push_back(Rect66by6one.get_id());

        Agent& Rect6by186one = add_agent("Rect_6_186", 0, 90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by186one: " << Rect6by186one.get_id() << "\n";
        agent_ids.push_back(Rect6by186one.get_id());

        Agent& Rect6by66one = add_agent("Rect_6_66", -60, -90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66one: " << Rect6by66one.get_id() << "\n";
        agent_ids.push_back(Rect6by66one.get_id());

        Agent& Rect6by66two = add_agent("Rect_6_66", 0, 30, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66two: " << Rect6by66two.get_id() << "\n";
        agent_ids.push_back(Rect6by66two.get_id());

        Agent& Rect6by66three = add_agent("Rect_6_66", 60, -90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66three: " << Rect6by66three.get_id() << "\n";
        agent_ids.push_back(Rect6by66three.get_id());
        
        return agent_ids;
    }

    vector<int> build_maze_3() {
        vector<int> agent_ids;
        Agent& Rect186by6one = add_agent("Rect_6_186", -90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect186by6one: " << Rect186by6one.get_id() << "\n";
        agent_ids.push_back(Rect186by6one.get_id());

        Agent& Rect186by6two = add_agent("Rect_6_186", 90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect186by6two: " << Rect186by6two.get_id() << "\n";
        agent_ids.push_back(Rect186by6two.get_id());

        Agent& Rect6by186one = add_agent("Rect_6_186", 0, -90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by186one: " << Rect6by186one.get_id() << "\n";
        agent_ids.push_back(Rect6by186one.get_id());

        Agent& Rect6by66one = add_agent("Rect_6_66", -60, 90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66one: " << Rect6by66one.get_id() << "\n";
        agent_ids.push_back(Rect6by66one.get_id());

        Agent& Rect6by66two = add_agent("Rect_6_66", 60, 90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66two: " << Rect6by66two.get_id() << "\n";
        agent_ids.push_back(Rect6by66two.get_id());

        Agent& Rect6by66three = add_agent("Rect_6_66", 0, 30, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66one: " << Rect6by66three.get_id() << "\n";
        agent_ids.push_back(Rect6by66three.get_id());

        Agent& Rect6by66four = add_agent("Rect_6_66", 0, -30, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66one: " << Rect6by66four.get_id() << "\n";
        agent_ids.push_back(Rect6by66four.get_id());

        Agent& Rect66by6one = add_agent("Rect_6_66", -30, 60, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect66by6one: " << Rect66by6one.get_id() << "\n";
        agent_ids.push_back(Rect66by6one.get_id());

        Agent& Rect66by6two = add_agent("Rect_6_66", 30, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect66by6two: " << Rect66by6two.get_id() << "\n";
        agent_ids.push_back(Rect66by6two.get_id());
        return agent_ids;
    }

    vector<int> build_maze_4() {
        vector<int> agent_ids;
        Agent& Rect306by6one = add_agent("Rect_6_306", -150, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect306by6one: " << Rect306by6one.get_id() << "\n";
        agent_ids.push_back(Rect306by6one.get_id());

        Agent& Rect306by6two = add_agent("Rect_6_306", 150, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect306by6two: " << Rect306by6two.get_id() << "\n";
        agent_ids.push_back(Rect306by6two.get_id());

        Agent& Rect186by6one = add_agent("Rect_6_186", -30, -60, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect186by6one: " << Rect186by6one.get_id() << "\n";
        agent_ids.push_back(Rect186by6one.get_id());

        Agent& Rect186by6two = add_agent("Rect_6_186", 90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect186by6two: " << Rect186by6two.get_id() << "\n";
        agent_ids.push_back(Rect186by6two.get_id());

        Agent& Rect126by6one = add_agent("Rect_6_126", 30, 90, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect126by6one: " << Rect126by6one.get_id() << "\n";
        agent_ids.push_back(Rect126by6one.get_id());

        Agent& Rect66by6one = add_agent("Rect_6_66", -90, 60, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect66by6one: " << Rect66by6one.get_id() << "\n";
        agent_ids.push_back(Rect66by6one.get_id());

        Agent& Rect66by6two = add_agent("Rect_6_66", 30, -120, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect66by6two: " << Rect66by6two.get_id() << "\n";
        agent_ids.push_back(Rect66by6two.get_id());

        Agent& Rect6by306one = add_agent("Rect_6_306", 0, 150, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by306one: " << Rect6by306one.get_id() << "\n";
        agent_ids.push_back(Rect6by306one.get_id());

        Agent& Rect6by126one = add_agent("Rect_6_126", -30, 90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by126one: " << Rect6by126one.get_id() << "\n";
        agent_ids.push_back(Rect6by126one.get_id());

        Agent& Rect6by126two = add_agent("Rect_6_126", -90, -150, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by126two: " << Rect6by126two.get_id() << "\n";
        agent_ids.push_back(Rect6by126two.get_id());

        Agent& Rect6by126three = add_agent("Rect_6_126", 90, -150, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by126three: " << Rect6by126three.get_id() << "\n";
        agent_ids.push_back(Rect6by126three.get_id());

        Agent& Rect6by66one = add_agent("Rect_6_66", -120, -30, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66one: " << Rect6by66one.get_id() << "\n";
        agent_ids.push_back(Rect6by66one.get_id());

        Agent& Rect6by66two = add_agent("Rect_6_66", -60, -90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66two: " << Rect6by66two.get_id() << "\n";
        agent_ids.push_back(Rect6by66two.get_id());

        Agent& Rect6by66three = add_agent("Rect_6_66", 0, -30, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66three: " << Rect6by66three.get_id() << "\n";
        agent_ids.push_back(Rect6by66three.get_id());

        Agent& Rect6by66four = add_agent("Rect_6_66", 60, -90, 0, {{"fill","black"},{"stroke","black"}});
        std::cout << "Adding Agent Rect6by66four: " << Rect6by66four.get_id() << "\n";
        agent_ids.push_back(Rect6by66four.get_id());

        return agent_ids;
    }

    vector<int> current_maze_agents_ids;
    int initial;
    int maze_number;
    int counter;
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
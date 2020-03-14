#ifndef __MAZE_ROBOT_AGENT__H
#define __MAZE_ROBOT_AGENT__H 

#include "enviro.h"
#include <iostream>
#include <math.h>

#define front_sensor sensor_value(0)
#define left_sensor sensor_value(2)
#define right_sensor sensor_value(1)

using namespace enviro;

class Stop : public State, public AgentInterface {
    public:
    void entry(const Event& e) {
        // std::cout << "Entered Stop\n";
        end_position_error = 30;
        end_position_x = 0; //-2.52039
        end_position_y = -180; //-177.507
    }
    void during() { 
            if ( (  position().x < end_position_x + end_position_error &&
                position().x > end_position_x - end_position_error ) &&
                position().y < end_position_y + end_position_error &&
                position().y > end_position_y - end_position_error ) {
                emit( Event( "Celebrate" ) );
            }
            if ( front_sensor > 35 ) {
                emit( Event( "Move Forward" ) );
            }
            if ( left_sensor > 35 ) {
                emit( Event( "Turn Left" ) );
            }
            if ( right_sensor > 35 ) {
                emit( Event( "Turn Right" ) );
            }
            if ( right_sensor < 35 && left_sensor < 35 && front_sensor < 35 ) {
                emit( Event( "Turn Around" ) );
            }
            
        }
    void exit(const Event& e) {}

    double end_position_x, end_position_y;
    double end_position_error;
};

class Begin : public State, public AgentInterface {
    public:
    void entry(const Event& e) {
        // std::cout << "Entered Begin\n";
        sleep(10);
    }
    void during() {
        emit( Event( "Stop" ) );
    }
    void exit(const Event& e) {}
};

class Celebrate : public State, public AgentInterface {
    public:
    void entry(const Event& e) {
        // std::cout << "Entered Celebrate\n";
        counter = 0;
        escape_angle = -90 * M_PI/180;
        once = 0;
    }
    void during() {
        if( counter < 20 ) {
            track_velocity(0,-300);
            counter ++;
        } else {
            if( angle() > -90 ) {
                track_velocity( 0, -1 );
            } else {
                track_velocity( 0, 1 );
            }
            if ( (  angle() > (escape_angle - 2 * M_PI/180 ) ) &&
                (  angle() < (escape_angle + 2 * M_PI/180 ) ) && once == 0 ) {
                track_velocity( 0, 0 );
                once ++;
            }
        }
    }
    void exit(const Event& e) {}

    int once;
    double counter;
    cpFloat escape_angle;
};

class MoveForward : public State, public AgentInterface {
    public:
    void entry(const Event& e) {
        // std::cout << "Entered MoveForward\n";
        entry_x = position().x;
        entry_y = position().y;
        entry_angle = angle();
        position_error = 2;
        angle_error = 45;
        angle_tune = 1;

        if ( ( angle() < 0 + angle_error * ( M_PI / 180 ) ) && ( angle() > 0 - angle_error * ( M_PI / 180 ) ) ) {
            //FACING EAST
            //position x + 60
            goal_x = entry_x + 60;
            goal_y = entry_y;
        } else if ( ( angle() < ( -90 + angle_error ) * ( M_PI / 180 ) ) && ( angle() > ( -90 - angle_error ) * ( M_PI / 180 ) ) ) {
            //FACING NORTH
            //postion y - 60
            goal_x = entry_x;
            goal_y = entry_y - 60;
        } else if ( ( angle() < ( -180 + angle_error ) * ( M_PI / 180 ) ) && ( angle() > ( -180 - angle_error ) * ( M_PI / 180 ) ) ) {
            //FACING WEST
            //postiion x - 60
            goal_x = entry_x - 60;
            goal_y = entry_y;
        } else if ( ( angle() < ( -270 + angle_error ) * ( M_PI / 180 ) ) && ( angle() > ( -270 - angle_error ) * ( M_PI / 180 ) ) ) {
            //FACING SOUTH
            //postion y + 60
            goal_x = entry_x ;
            goal_y = entry_y + 60;
        }  else if ( ( angle() < ( 90 + angle_error ) * ( M_PI / 180 ) ) && ( angle() > ( 90 - angle_error ) * ( M_PI / 180 ) ) ) {
            //FACING SOUTH
            //postion y + 60
            goal_x = entry_x ;
            goal_y = entry_y + 60;
        } else if ( ( angle() < ( 180 + angle_error ) * ( M_PI / 180 ) ) && ( angle() > ( 180 - angle_error ) * ( M_PI / 180 ) ) ) {
            //FACING WEST
            //postiion x - 60
            goal_x = entry_x - 60;
            goal_y = entry_y;
        } else if ( ( angle() < ( 270 + angle_error ) * ( M_PI / 180 ) ) && ( angle() > ( 270 - angle_error ) * ( M_PI / 180 ) ) ) {
            //FACING NORTH
            //postion y - 60
            goal_x = entry_x;
            goal_y = entry_y - 60;
        } else {
            // std::cout << "Could not determine which direction to move forward\n";
            goal_x = 0;
            goal_y = 0;
        }
    }
    void during() {
        //Move forward 60 places or until the sensors hit
        //move_toward(goal_x, goal_y, 100, 15);
        if (front_sensor > 20 ) {
            move_toward(goal_x, goal_y, 180, 20);
        } else {
            goal_x = position().x;
            goal_y = position().y;
        }

        if ( (  position().x < goal_x + position_error &&
                position().x > goal_x - position_error ) &&
                position().y < goal_y + position_error &&
                position().y > goal_y - position_error ) {
                    //make sure facing the right angle
                    // std::cout << "Within position error.\n";
                    // std::cout << "Position().x = " << position().x << ", Position().y = " << position().y << "\n";
                    if ( (  angle() > (entry_angle - angle_tune * M_PI/180 ) ) &&
                         (  angle() < (entry_angle + angle_tune * M_PI/180 ) ) ) {
                        track_velocity( 0, 0 );
                        emit( Event( "Stop" ) );
                    }
                    if( angle() > entry_angle ) {
                        // std::cout << "Tuning angle by turning counterclockwise\n";
                        track_velocity( 0, -1 );
                    } else {
                        // std::cout << "Tuning angle by turning clockwise\n";
                        track_velocity( 0, 1 );
                    }
                    //emit( Event( "Stop") );
                }
    }
    void exit(const Event& e) {}

    double position_error;
    cpFloat entry_angle;
    cpFloat angle_error;
    cpFloat angle_tune;
    double entry_x, entry_y;
    double goal_x, goal_y;
};

class TurnLeft : public State, public AgentInterface {
    public:
    void entry(const Event& e) {
        // std::cout << "Entered Turning Left\n";
        Entry_Angle = angle();
        Desired_Angle = Entry_Angle - ( 90 * M_PI/180 );
        // std::cout << "Desired angle: " << Desired_Angle * 180/M_PI << "\n";

        if( Desired_Angle < ( 45 * M_PI/180 ) && Desired_Angle > ( -45 * M_PI/180 ) ) {
            Desired_Angle = 0 * M_PI/180;
        } else if ( Desired_Angle > ( -135 * M_PI/180 ) && Desired_Angle < ( -45 * M_PI/180 ) ) {
            Desired_Angle = -90 * M_PI/180;
        } else if ( Desired_Angle > ( -225 * M_PI/180 ) && Desired_Angle < ( -135 * M_PI/180 ) ) {
            Desired_Angle = -180 * M_PI/180;
        } else if ( Desired_Angle > ( -315 * M_PI/180 ) && Desired_Angle < ( -225 * M_PI/180 ) ) {
            Desired_Angle = -270 * M_PI/180;
        } else if ( Desired_Angle > ( 45 * M_PI/180 ) && Desired_Angle < ( 135 * M_PI/180 ) ) {
            Desired_Angle = 90 * M_PI/180;
        } else if (  Desired_Angle > ( 135 * M_PI/180 ) && Desired_Angle < ( 225 * M_PI/180 ) ) {
            Desired_Angle = 180 * M_PI/180;
        } else if ( Desired_Angle > ( 225 * M_PI/180 ) && Desired_Angle < ( 315 * M_PI/180 ) ) {
            Desired_Angle = 270 * M_PI/180;
        } else {
            Desired_Angle = 0 * M_PI/180;
            // std::cout << "Could not determine angle to choose.\n";
        }

        // std::cout << "Recalculated Desired angle: " << Desired_Angle * 180/M_PI << "\n";

        Current_Angle = Entry_Angle;
    }
    void during() {
        //Turn the robot mostly left and then move forward
        Current_Angle = angle();
        if ( (  Current_Angle > (Desired_Angle - 0.1 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 0.1 * M_PI/180 ) ) ) {
                track_velocity( 0, 0 );
                emit( Event( "Stop" ) );
        }
        if ( (  Current_Angle > (Desired_Angle - 1 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 1 * M_PI/180 ) ) ) {
                if( Current_Angle > Desired_Angle) {
                    track_velocity( 0, -0.1 );
                } else {
                    track_velocity( 0, 0.1 );
                }
        }
        if ( (  Current_Angle > (Desired_Angle - 5 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 5 * M_PI/180 ) ) ) {
                if( Current_Angle > Desired_Angle) {
                    track_velocity( 0, -0.8 );
                } else {
                    track_velocity( 0, 0.8 );
                }
        }
        if( Current_Angle > Desired_Angle) {
            track_velocity( 0, -2 );
        } else {
            track_velocity( 0, 2 );
        }
        
        
    }
    void exit(const Event& e) {}    

    cpFloat Current_Angle;
    cpFloat Desired_Angle;
    cpFloat Entry_Angle;
};

class TurnRight : public State, public AgentInterface {
    public:
    void entry(const Event& e) {
        // std::cout << "Entered Turning Right\n";
        Entry_Angle = angle();
        Desired_Angle = Entry_Angle + ( 90 * M_PI/180 );

        // std::cout << "Desired angle: " << Desired_Angle * 180/M_PI << "\n";

        if( Desired_Angle < ( 45 * M_PI/180 ) && Desired_Angle > ( -45 * M_PI/180 ) ) {
            Desired_Angle = 0 * M_PI/180;
        } else if ( Desired_Angle > ( -135 * M_PI/180 ) && Desired_Angle < ( -45 * M_PI/180 ) ) {
            Desired_Angle = -90 * M_PI/180;
        } else if ( Desired_Angle > ( -225 * M_PI/180 ) && Desired_Angle < ( -135 * M_PI/180 ) ) {
            Desired_Angle = -180 * M_PI/180;
        } else if ( Desired_Angle > ( -315 * M_PI/180 ) && Desired_Angle < ( -225 * M_PI/180 ) ) {
            Desired_Angle = -270 * M_PI/180;
        } else if ( Desired_Angle > ( 45 * M_PI/180 ) && Desired_Angle < ( 135 * M_PI/180 ) ) {
            Desired_Angle = 90 * M_PI/180;
        } else if (  Desired_Angle > ( 135 * M_PI/180 ) && Desired_Angle < ( 225 * M_PI/180 ) ) {
            Desired_Angle = 180 * M_PI/180;
        } else if ( Desired_Angle > ( 225 * M_PI/180 ) && Desired_Angle < ( 315 * M_PI/180 ) ) {
            Desired_Angle = 270 * M_PI/180;
        } else {
            Desired_Angle = 0 * M_PI/180;
            // std::cout << "Could not determine angle to choose.\n";
        }

        // std::cout << "Recalculated Desired angle: " << Desired_Angle * 180/M_PI << "\n";

        Current_Angle = Entry_Angle;
    }
    void during() {
        //Turn the robot mostly left and then move forward
        Current_Angle = angle();
        if ( (  Current_Angle > (Desired_Angle - 0.1 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 0.1 * M_PI/180 ) ) ) {
                track_velocity( 0, 0 );
                emit( Event( "Stop" ) );
        }
        if ( (  Current_Angle > (Desired_Angle - 1 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 1 * M_PI/180 ) ) ) {
                if( Current_Angle < Desired_Angle) {
                    track_velocity( 0, 0.1 );
                } else {
                    track_velocity( 0, -0.1 );
                }
        }
        if ( (  Current_Angle > (Desired_Angle - 5 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 5 * M_PI/180 ) ) ) {
                if( Current_Angle < Desired_Angle) {
                    track_velocity( 0, 0.8 );
                } else {
                    track_velocity( 0, -0.8 );
                }
        }
        if( Current_Angle < Desired_Angle) {
            track_velocity( 0, 2 );
        } else {
            track_velocity( 0, -2 );
        }
        
        
    }
    void exit(const Event& e) {}    

    cpFloat Current_Angle;
    cpFloat Desired_Angle;
    cpFloat Entry_Angle;
};

class TurnAround : public State, public AgentInterface {
    public:
    void entry(const Event& e) {
        // std::cout << "Entered Turning Around\n";
        Entry_Angle = angle();
        Desired_Angle = Entry_Angle - ( 180 * M_PI/180 );

        // std::cout << "Desired angle: " << Desired_Angle * 180/M_PI << "\n";

        if( Desired_Angle < ( 45 * M_PI/180 ) && Desired_Angle > ( -45 * M_PI/180 ) ) {
            Desired_Angle = 0 * M_PI/180;
        } else if ( Desired_Angle > ( -135 * M_PI/180 ) && Desired_Angle < ( -45 * M_PI/180 ) ) {
            Desired_Angle = -90 * M_PI/180;
        } else if ( Desired_Angle > ( -225 * M_PI/180 ) && Desired_Angle < ( -135 * M_PI/180 ) ) {
            Desired_Angle = -180 * M_PI/180;
        } else if ( Desired_Angle > ( -315 * M_PI/180 ) && Desired_Angle < ( -225 * M_PI/180 ) ) {
            Desired_Angle = -270 * M_PI/180;
        } else if ( Desired_Angle > ( 45 * M_PI/180 ) && Desired_Angle < ( 135 * M_PI/180 ) ) {
            Desired_Angle = 90 * M_PI/180;
        } else if (  Desired_Angle > ( 135 * M_PI/180 ) && Desired_Angle < ( 225 * M_PI/180 ) ) {
            Desired_Angle = 180 * M_PI/180;
        } else if ( Desired_Angle > ( 225 * M_PI/180 ) && Desired_Angle < ( 315 * M_PI/180 ) ) {
            Desired_Angle = 270 * M_PI/180;
        } else {
            Desired_Angle = 0 * M_PI/180;
            // std::cout << "Could not determine angle to choose.\n";
        }

        // std::cout << "Recalculated Desired angle: " << Desired_Angle * 180/M_PI << "\n";

        Current_Angle = Entry_Angle;
    }
    void during() {
        //Turn the robot mostly left and then move forward
        Current_Angle = angle();
        if ( (  Current_Angle > (Desired_Angle - 0.1 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 0.1 * M_PI/180 ) ) ) {
                track_velocity( 0, 0 );
                emit( Event( "Stop" ) );
        }
        if ( (  Current_Angle > (Desired_Angle - 1 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 1 * M_PI/180 ) ) ) {
                if( Current_Angle < Desired_Angle) {
                    track_velocity( 0, 0.1 );
                } else {
                    track_velocity( 0, -0.1 );
                }
        }
        if ( (  Current_Angle > (Desired_Angle - 5 * M_PI/180 ) ) &&
             (  Current_Angle < (Desired_Angle + 5 * M_PI/180 ) ) ) {
                if( Current_Angle < Desired_Angle) {
                    track_velocity( 0, 0.8 );
                } else {
                    track_velocity( 0, -0.8 );
                }
        }
        if( Current_Angle < Desired_Angle) {
            track_velocity( 0, 2 );
        } else {
            track_velocity( 0, -2 );
        }
        
        
    }
    void exit(const Event& e) {}    

    cpFloat Current_Angle;
    cpFloat Desired_Angle;
    cpFloat Entry_Angle;
};

class MazeRobotController : public StateMachine, public AgentInterface {

    public:
    MazeRobotController() : StateMachine() {
        set_initial(begin);
        add_transition("Stop", begin, stop);
        add_transition("Turn Around", stop, turn_around);
        add_transition("Turn Left", stop, turn_left);
        add_transition("Turn Right", stop, turn_right);
        add_transition("Move Forward", stop, move_forward);
        add_transition("Stop", move_forward, stop);
        add_transition("Stop", turn_left, stop);
        add_transition("Stop", turn_right, stop);
        add_transition("Stop", turn_around, stop);
        add_transition("Celebrate", stop, celebrate);
    }

    MoveForward move_forward;
    TurnAround turn_around;
    TurnLeft turn_left;
    TurnRight turn_right;
    Stop stop;
    Celebrate celebrate;
    Begin begin;
};

class MazeRobot : public Agent {
    public:
    MazeRobot(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    MazeRobotController c;
};

DECLARE_INTERFACE(MazeRobot)

#endif
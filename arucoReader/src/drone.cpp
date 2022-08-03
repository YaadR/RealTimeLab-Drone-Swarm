#include "drone.h"
#include "arucoReader"

class drone{
    private:
        /* data */
        int id;

    public:
        drone(/* args */);
        ~drone();
    
    drone::drone(/* args */){}
    
    drone::~drone(){}
    
    data get_leader_position(){
        data positionChange;
        arucoReaderRun();
        return positionChange;
    }
    void move_drone(data positionChange){
         
        return;
    }

    void run(){
        while (true){
            move_drone(get_leader_position());
        }
    }
};
   

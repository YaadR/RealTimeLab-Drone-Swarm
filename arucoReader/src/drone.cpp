#include "drone.h"
#include "arucoReader"
#include "arocuLeaderInfo.h"


drone::drone(/* args */){}

drone::~drone(){}

arocuLeaderInfo drone::get_leader_position(){
    arocuLeaderInfo positionChange;
    positionChange = arucoReaderRun();
    return positionChange;
}
void drone::move_drone(arocuLeaderInfo positionChange){
        
    return;
}

void drone::run(){
}


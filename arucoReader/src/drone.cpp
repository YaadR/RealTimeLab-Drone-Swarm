#include "drone.h"
#include "arucoReader"
#include "arocuLeaderInfo.h"


drone::drone(aruco currentInfo){
	this->currentInfo = currentInfo;
}

// drone::~drone(){}

aruco drone::get_leader_position(){
    aruco positionChange;
    positionChange = arucoReaderRun();
    return positionChange;
}

void drone::move_drone(aruco positionChange){
    int rightDrone = positionChange.rightLeft;
    
    if (rightDrone >= 0) {
        /* move to
        Forward = 1.0;
        rightLeft = 0.3;
        upDown = 0;
        angle = 0;
        clockwise: 0 ;
        */
    }
    else {
        /* move to
        Forward = 1.37;
        rightLeft = -0.3;
        upDown = 0;
        angle = 0;
        clockwise = 0 ;
        */    
    }

    return;
}

void drone::run(){
	/*
	while (true){
        	move_drone();
	}
    	*/
}

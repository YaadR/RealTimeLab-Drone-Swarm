#include <unistd.h>
#include "drone.h"
#include <iostream>
// #include "arucoReader"
// #include "arocuLeaderInfo.h"


drone::drone(){}

drone::~drone(){}

void drone::addInfo(aruco &origin){
	this->upDown = origin.upDown;
    	this->forward = origin.forward;
    	this->rightLeft = origin.rightLeft;
    	this->idr = origin.idr;
    	this->ifArucoExist = origin.ifArucoExist;
    	this->clockwise = origin.leftOverAngle.second;
    	this->angle = origin.leftOverAngle.first;
    	// this-> leftOverAngle = origin.leftOverAngle;
    	// this->runCamera = origin.runCamera;
    	/*std::cout 
        << "forward: " << this->forward 
        << " right left: " << this->rightLeft 
        << " updown: " << this->upDown
        << " id: " << this->idr 
        << " read Aruco: " << this->ifArucoExist
        << std::endl;*/
}

void drone::get_leader_position(){
    aruco positionChange;
    positionChange = arucoReaderRun();
    return positionChange;
}

void drone::move_drone(){
    if(ifArucoExist == 1){
	    if(clockwise == 0) {
	    	// move to:
		// Forward = 1.0;
		// rightLeft = 0.3;
		// upDown = 0;   
		
		std::cout << "First Drone, tello.SendCommand(rc " 
	    	<< rightLeft - (-0.3) << " , " 
	    	<< forward - 1.0 << " , " 
	    	<< upDown - 0 << " , "
	    	<< angle - (-0.3) << ")"
	    	<< std::endl << std::endl;
		
	    }
	    else {
	    	// move to:
		// Forward = 1.37;
		// rightLeft = -0.3;
		// upDown = 0;
	    	
	    	std::cout << "Second Drone, tello.SendCommand(rc " 
	    	<< rightLeft - (0.3)  << " , " 
	    	<< forward - 1.37 << " , " 
	    	<< upDown - 0 << " , "
	    	<< angle - (0.3) << ")"
	    	<< std::endl << std::endl;
	    }
	}
	else{
		std::cout << "waiting for data" << std::endl;
	}
    return;
}

void drone::run(){
	
    while (true) {
        get_leader_position();
        move_drone();
        }
    /*
    thread1 -> while (true) {
        get_leader_position();
        }
    thread2 -> while (true){
        	move_drone();
	        }       
    	*/
}

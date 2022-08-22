#include <unistd.h>
#include "drone.h"
#include <iostream>
#include <string.h>
// #include "runAruco"

// #include "arucoReader"
// #include "arocuLeaderInfo.h"

/*@Y*/
float MAX_DIST=1.5;
float MIN_DIST=0.4;
float  MAX_LEFT_RIGHT=1;
float  MIN_LEFT_RIGHT=0.1;
float  UP_DOWN_RANGE=0.4;
float  YAW_RANGE=35;
float  ROTATE_RANGE=0.1;
int ARUCO_DATA_SIZE=14;
/*@Ye*/

drone::drone(){
	
}

void drone::setRightOrLeft(int i){          //check if still relevant    
	this->RightOrLeft = i;
}

double drone::getRightLeft(aruco &detector){    //check if still relevant
	return detector.rightLeft;
}

bool drone::getRightOrLeft(){
	return this->RightOrLeft;
}

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

// drone::~drone(){}

/*std::vector <auto> drone::get_leader_position(){
    return this->ourAruco->get_info();
}*/

/*void drone::set_info(std::vector <auto> rawInfo){
    this->rightLeft = rawInfo[0];//.rightLeft;                      //need to make enum
    this->forward = rawInfo[1];//.forward;
    this->upDown = rawInfo[2];//.upDown;
    this->angle = rawInfo[3];//.leftOverAngle.first;
}*/

std::string drone::move_drone(){

    /*@Y*/
    float v_data[ARUCO_DATA_SIZE] = {0};
    v_data[0] = (float)this->forward;
    v_data[1] = (float)this->rightLeft;
    v_data[2] = (float)this->upDown;
    v_data[3] = (float)this->angle;
    v_data[4] = (float)this->clockwise;
    v_data[5] = (float)this->RightOrLeft;
    v_data[6] = (float)this->ifArucoExist;
    /*
    std::vector<double> dat_vec;
    dat_vec.push_back((double)this->forward);
    dat_vec.push_back((double)this->rightLeft);
    dat_vec.push_back((double)this->upDown);
    dat_vec.push_back((double)this->angle);
    dat_vec.push_back((double)this->clockwise);
    dat_vec.push_back((double)this->idr);
    dat_vec.push_back((double)this->ifArucoExist);
    /**/
    
    /*
	flags[0], v_data[0] = forward
	flags[1], v_data[1] = rightLeft
	flags[2], v_data[2] = upDown
	flags[3], v_data[3] = angle
	flags[4], v_data[4] = clockwise
	flags[5], v_data[5] = idr
	flags[6], v_data[6] = ifArucoExist
	*/
    //int flags[ARUCO_DATA_SIZE] = {0}; 
    std::string flags = "rc +0 +0 +0 +0"; // "rc Left/Right Forward/Backward Up/Down Yaw"
	
    if(v_data[6]){
        if(v_data[0]<MIN_DIST || v_data[0]>MAX_DIST){
            if(v_data[0]<MIN_DIST){
                std::cout << "Move Backward" << std::endl;
                flags[7]='5';
                flags[6]='-';
                
            }else{
                std::cout << "Move Forward" << std::endl;
                flags[7]='5';
            }
        }

        if((abs(v_data[1])>MAX_LEFT_RIGHT)||(abs(v_data[1]) < MIN_LEFT_RIGHT)){
            switch((int)v_data[5]%2){        // Even -> 0 -> Left || Odd -> 1 -> Right
                case 0:
                    if(v_data[1]<-MAX_LEFT_RIGHT){
                        std::cout << "Move Right" << std::endl;
                        flags[4]= '5';
                    }else{
                        std::cout << "Move Left" << std::endl;
                        flags[4]= '5';
                        flags[3]='-';
                    }                  
                    break;
                case 1:
                    if(v_data[1]<MIN_LEFT_RIGHT){
                        std::cout << "Move Right" << std::endl;
                        flags[4]= '5';
                    }else{
                        std::cout << "Move Left" << std::endl;
                        flags[4]= '5';
                        flags[3]='-';
                    }                 
                    break;
                default:
                    break;
            }

        }

        if(abs(v_data[2])>UP_DOWN_RANGE)
        {
            if(v_data[2]>UP_DOWN_RANGE)
            {
                std::cout << "Move Down" << std::endl;
                flags[9]='-';
                flags[10]='5';
            }else{
                std::cout << "Move Up" << std::endl;
                flags[10]='5';
            }
        }

        if(abs(v_data[3])>YAW_RANGE)
        {
            if(v_data[3]>YAW_RANGE){
                std::cout << "Yaw right" << std::endl;
                flags[13]='5';
            }else{
                std::cout << "Yaw left" << std::endl;
                flags[12]='-';
                flags[13]='5';
            }
        }

    }else{
    	std::cout<< "waiting for data" << std::endl;
    }
    std::cout << flags << std::endl;
    //tello.SendCommand(flags)
    return flags;


    /*@Ye*/
    /*
    if(ifArucoExist == 1){
	    if(clockwise == 0) {
	    	// move to:
		// Forward = 1.0;
		// rightLeft = -0.3;
		// yaw = 0;   
		
		std::cout << "First Drone, tello.SendCommand(rc " 
	    	<< (rightLeft - (-0.3)) * 100 << " , " 
	    	<< (forward - 1.0) * 100 << " , " 
	    	<< (upDown - 0) * 100 << " , "
	    	<< (angle) << ")"
	    	<< std::endl << std::endl;
		
	    }
	    else {
	    	// move to:
		// Forward = 1.37;
		// rightLeft = 0.3;
		// upDown = 0;
		// yaw = 0
	    	
	    	std::cout << "Second Drone, tello.SendCommand(rc " 
	    	<< (rightLeft - (0.3)) * 100  << " , " 
	    	<< (forward - 1.37) * 100 << " , " 
	    	<< (upDown - 0)  * 100 << " , "
	    	<< (angle)<< ")"
	    	<< std::endl << std::endl;
	    }
	}
	else{
		std::cout << "waiting for data" << std::endl;
	}/**/
}

/*void set_id(int id){
    if(id < 0)                         
        	myDrone->id = 0; // left drone
        else
        	myDrone->id = 1; // right drone
}*/

void drone::runDrone(/*aruco &origin*/){
		//tzukArucoReaderRun(this);
        //this.set_id(ourAruco.get_id());

        //implement multi thread        
        /*while(true){
			this->set_info(this.get_leader_position());
            this.move_drone();
		}*/


		/*std::thread th1(addInfo, ourDrone);
    		std::thread th2(move_drone, ourDrone);
    
    		th1.join();
    		th2.join();*/
	
    	
}



/////////// Logic implamentation from here /////////////////



/*
void drone::move_to(float *v_data){

    
}

*/
 

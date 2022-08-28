#include <unistd.h>
#include "drone.h"
#include <iostream>
#include <string.h>
#include <thread>
// #include "runAruco"

// #include "arucoReader"
// #include "arocuLeaderInfo.h"

/*@Y*/
float MAX_DIST=1.3;
float MIN_DIST=1.1;
float  MAX_LEFT_RIGHT=0.2;
float  MIN_LEFT_RIGHT=0.1;
float  UP_DOWN_RANGE=0.1;
float  YAW_RANGE=25;
int ARUCO_DATA_SIZE=18;
float THE_CONST = 10.0;
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
    	//this->idr = origin.idr;
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
        << std::endl;/**/
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
	int relative_const(float bigger, float smaller){
		
		return (int)(((abs(bigger)+0.2)/(abs(smaller)+0.2))*THE_CONST);
		
	}

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
    std::string flags = "rc  00  00  00  00"; // "rc Left/Right Forward/Backward Up/Down Yaw"
    int dis = 00;
    std::string string_dis;
	/**/
	
    if(v_data[6]){
        if(v_data[0]<MIN_DIST || v_data[0]>MAX_DIST){
            if(v_data[0]<MIN_DIST){
                dis = relative_const(MIN_DIST,v_data[0]);
                std::cout << dis << std::endl;
                flags[8]= dis[0];
                flags[9]= dis[1];
                flags[7]='-';
                
            }else{
                dis = relative_const(v_data[0],MAX_DIST);
                //std::cout << dis << std::endl;
                flags[8]= dis[0];
                flags[9]= dis[1];
            }
        }

        if((abs(v_data[1])>MAX_LEFT_RIGHT)||(abs(v_data[1]) < MIN_LEFT_RIGHT)){
            switch((int)v_data[5]%2){        // Even -> 0 -> Left || Odd -> 1 -> Right
                case 1:
                    if(v_data[1] > -MIN_LEFT_RIGHT /*|| v_data[1] < -MAX_LEFT_RIGHT*/){
                        dis = relative_const(MIN_LEFT_RIGHT,v_data[1]);
                        //std::cout << dis << std::endl;
                        flags[4]= dis[0];
                        flags[5]= dis[1];
                        
                    }else{
                        dis = relative_const(v_data[1],MAX_LEFT_RIGHT);
                        //std::cout << dis << std::endl;
                        flags[4]= dis[0];
                        flags[5]= dis[1];
                        flags[3]='-';
                    }                  
                    break;
                case 0:
                    if(/*v_data[1] < MIN_LEFT_RIGHT ||*/ v_data[1] > MAX_LEFT_RIGHT){
                        dis = relative_const(MIN_LEFT_RIGHT,v_data[1]);
                        //std::cout << dis << std::endl;
                        flags[4]= dis[0];
                        flags[5]= dis[1];
                        flags[3]='-';
                    }else{
                        dis = relative_const(v_data[1],MAX_LEFT_RIGHT);
                	//std::cout << dis << std::endl;                
                        flags[4]= dis[0];
                        flags[5]= dis[1];
                        
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
            
            	flags[11]='-';
                dis = 2 * relative_const(v_data[2],UP_DOWN_RANGE); 
                string_dis = std::to_string(dis);        
                flags[12]= string_dis[0];
                flags[13]= string_dis[1];

            }else{
                dis = relative_const(v_data[2],UP_DOWN_RANGE);
                
                flags[12]= dis[0];
                flags[13]= dis[1];
            }
        }

        if(v_data[3]>YAW_RANGE )
        {
            if(v_data[4]==1){
                //std::cout << "Yaw right" << std::endl;
                flags[15]='-';
                flags[17]='5';
            }else{ 
                flags[17]='5';
            }
        }

    }
    return flags;


    /*@Ye*/
   
}

/*void set_id(int id){
    if(id < 0)                         
        	myDrone->id = 0; // left drone
        else
        	myDrone->id = 1; // right drone
}*/
/*
void drone::runDrone(/*aruco &origin){
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
	
    	
/*}
*/


/////////// Logic implamentation from here /////////////////



/*
void drone::move_to(float *v_data){

    
}

*/
 

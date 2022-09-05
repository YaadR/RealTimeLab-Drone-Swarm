#include <unistd.h>
#include "drone.h"
#include <iostream>
#include <string.h>
#include <thread>
#include <math.h>


float MAX_DIST_LEFT = 1.5;
float MIN_DIST_LEFT = 1.1;

float MAX_DIST_RIGHT = 1.8;
float MIN_DIST_RIGHT = 1.4;

float MAX_LEFT_RIGHT = 0.3;
float MIN_LEFT_RIGHT = 0.15;


float UP_RANGE = 10; //0.1;
float DOWN_RANGE = -15; //-0.1;

float YAW_RANGE = 15;

int ARUCO_DATA_SIZE = 18;

float THE_CONST = 10.0;

int POWER_SCALE_MIN = 30;
int POWER_SCALE_MAX = 15;

int equalizer[4] ={0};
float MINN = 0;
float  MAXX = 0;

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
	//this->upDown = origin.upDown;
    	this->forward = origin.forward;
    	this->rightLeft = origin.rightLeft;
    	this->ifArucoExist = origin.ifArucoExist;
    	this->clockwise = origin.leftOverAngle.second;
    	this->angle = origin.leftOverAngle.first;
    	//this->rollAngle = origin.rollAngle
	if(origin.rollAngle > 0){
		this->upDown = origin.upDown*100 + (180 - origin.rollAngle);
	} else {
		this->upDown = origin.upDown*100 - (180 + origin.rollAngle);
	}

}
void ret_min_max(float curr){
	MAXX = MAXX < curr ? curr:MAXX;
	MINN = MINN > curr ? curr:MINN;
}
int min_ret(float a, float b){
		return int(a>=b?b:a);

	}
int max_ret(float a, float b){
	return int(a<b?b:a);
}
int relative_const(float bigger, float smaller){
		
		//return max_ret(POWER_SCALE_MAX,min_ret(POWER_SCALE_MIN,((std::abs(bigger)+0.2)/(std::abs(smaller)+0.2))*THE_CONST));
	
	return max_ret(POWER_SCALE_MAX,min_ret(POWER_SCALE_MIN,(abs(bigger-smaller)*THE_CONST)));
	}



//float check_bounderies(float current, float left, float right, float scale = 1){
//	if (current <	
//}

//std::string drone::move_drone(){
int* drone::move_drone(){
    float v_data[ARUCO_DATA_SIZE] = {0};
    v_data[0] = (float)this->forward;
    v_data[1] = (float)this->rightLeft;
    v_data[2] = (float)this->upDown;
    v_data[3] = (float)this->angle;
    v_data[4] = (float)this->clockwise;
    v_data[5] = (float)this->RightOrLeft;
    v_data[6] = (float)this->ifArucoExist;

    
    /*
	flags[0], v_data[0] = forward
	flags[1], v_data[1] = rightLeft
	flags[2], v_data[2] = upDown
	flags[3], v_data[3] = angle
	flags[4], v_data[4] = clockwise
	flags[5], v_data[5] = idr
	flags[6], v_data[6] = ifArucoExist
	*/
    	// "rc Left/Right Forward/Backward Up/Down Yaw"

	
	equalizer[0] = 0;
	equalizer[1] = 0;
	equalizer[2] = 0;
	equalizer[3] = 0;
	/**/
	
    if(v_data[6]){
        if((abs(v_data[1])>MAX_LEFT_RIGHT)||(abs(v_data[1]) < MIN_LEFT_RIGHT)){
            switch((int)v_data[5]%2){        // Even -> 0 -> Left || Odd -> 1 -> Right
                case 1:
                    if(v_data[1] > -MIN_LEFT_RIGHT){
                        equalizer[0]= int(-relative_const(MIN_LEFT_RIGHT,v_data[1])*1.2);
                    }else if(v_data[1] < -MAX_LEFT_RIGHT){
                        equalizer[0]= int(relative_const(v_data[1],MAX_LEFT_RIGHT)*1.2);
                    }                  
                    break;
                case 0:
                    if(v_data[1] > MAX_LEFT_RIGHT){
                        equalizer[0]= int(-relative_const(MIN_LEFT_RIGHT,v_data[1])*1.2);
                    }else if(v_data[1] < MIN_LEFT_RIGHT){
                        equalizer[0]= int(relative_const(v_data[1],MAX_LEFT_RIGHT)*1.2);        
                    }                 
                    break;
                default:
                    break;
            }
        }
        
        switch((int)v_data[5]%2){
        case 1:
		if(v_data[0]<MIN_DIST_LEFT || v_data[0]>MAX_DIST_LEFT){
		    if(v_data[0]<MIN_DIST_LEFT){
		        equalizer[1]= int(-relative_const(MIN_DIST_LEFT,v_data[0])*1.5);		        
		    }else{
			equalizer[1]= int(relative_const(v_data[0],MAX_DIST_LEFT)*1.5);
		    }
		}
		break;
	case 0:
		if(v_data[0]<MIN_DIST_RIGHT || v_data[0]>MAX_DIST_RIGHT){
		    if(v_data[0]<MIN_DIST_RIGHT){
		        equalizer[1]= int(-relative_const(MIN_DIST_RIGHT,v_data[0])*1.5);
		    }else{
			equalizer[1]= int(relative_const(v_data[0],MAX_DIST_RIGHT)*1.5);
		    }
		}
		break;
		}
	
        if(v_data[2]>UP_RANGE || v_data[2] < DOWN_RANGE)
        {
            
            std::cout << v_data[2] << std::endl;
            if(v_data[2]>UP_RANGE)
            {
       		equalizer[2]= -int(relative_const(v_data[2],UP_RANGE));
            }else{
       		equalizer[2]= int(relative_const(v_data[2],DOWN_RANGE));
            }
        }
               
        
	if(v_data[3]>YAW_RANGE)
        {
            if(v_data[4]==1){
		equalizer[3] = 1.5*relative_const(v_data[3],10);
            }else{ 
       
		equalizer[3] = (-1.5)*relative_const(v_data[3],10);              
            }
        }
        if(v_data[3]>YAW_RANGE*2)
        {
            if(v_data[4]==1){
            	equalizer[1] = -25;
		equalizer[3] = 1.5*relative_const(v_data[3],15);
            }else{ 
       		equalizer[1] = -25;
		equalizer[3] = (-1.5)*relative_const(v_data[3],15);              
            }
        }
        

        
    }else{
    	equalizer[0] = -1;
    	equalizer[1] = -1;
    	equalizer[2] = -1;
    	equalizer[3] = -1;
    }
    return equalizer;

}


 

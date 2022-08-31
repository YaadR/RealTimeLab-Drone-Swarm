#include <unistd.h>
#include "drone.h"
#include <iostream>
#include <string.h>
#include <thread>
#include <math.h>


float MAX_DIST=1;
float MIN_DIST=0.8;
float  MAX_LEFT_RIGHT=0.3;
float  MIN_LEFT_RIGHT=0.15;
float  UP_DOWN_RANGE=0.1;
float  YAW_RANGE=30;
int ARUCO_DATA_SIZE=18;
float THE_CONST = 10.0;
int equalizer[4] ={0};
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
    	this->ifArucoExist = origin.ifArucoExist;
    	this->clockwise = origin.leftOverAngle.second;
    	this->angle = origin.leftOverAngle.first;

}

int min_ret(float a, float b){
		return int(a>=b?b:a);
	}
	int relative_const(float bigger, float smaller){
		
		return min_ret(30,((std::abs(bigger)+0.2)/(std::abs(smaller)+0.2))*THE_CONST);
	}

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
    std::string flags = "rc "; // "rc Left/Right Forward/Backward Up/Down Yaw"
    int dis = 0;

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
                        dis = relative_const(MIN_LEFT_RIGHT,v_data[1]);
                        dis = int(dis*1.5);
                        equalizer[0]= -dis;
                        flags+= std::to_string(-dis);

                        
                    }else if(v_data[1] < -MAX_LEFT_RIGHT){
                    	dis = relative_const(v_data[1],MAX_LEFT_RIGHT);
                        dis = int(dis*1.5);
                        equalizer[0]= dis;
                        flags+= std::to_string(dis);
                    }                  
                    break;
                case 0:
                    if(v_data[1] > MAX_LEFT_RIGHT){
                        dis = relative_const(MIN_LEFT_RIGHT,v_data[1]);
                        dis = int(dis*1.5);
                        equalizer[0]= -dis;
                        flags+= std::to_string(-dis);

                    }else if(v_data[1] < MIN_LEFT_RIGHT){
 
                        dis = relative_const(v_data[1],MAX_LEFT_RIGHT);
                        dis = int(dis*1.5);
                        equalizer[0]= dis;
                        flags+= std::to_string(dis);              
                    }                 
                    break;
                default:
                    break;
            }
        }
        flags+= " ";
        if(v_data[0]<MIN_DIST || v_data[0]>MAX_DIST){
            if(v_data[0]<MIN_DIST){
                dis = relative_const(MIN_DIST,v_data[0]);
                dis = int(dis*1.5);
                equalizer[1]= -dis; 
                flags+= std::to_string(-dis); 
                
            }else{
                dis = relative_const(v_data[0],MAX_DIST);
		dis = int(dis*1.5);
		equalizer[1]= dis; 
                flags+= std::to_string(dis); 
            }
        }
	flags+= " ";
        if(abs(v_data[2])>UP_DOWN_RANGE)
        {
            if(v_data[2]>UP_DOWN_RANGE)
            {
                dis = relative_const(v_data[2],UP_DOWN_RANGE); 
       		equalizer[2]= -dis;
		flags+= std::to_string(-dis);

            }else{
                dis = relative_const(v_data[2],UP_DOWN_RANGE);
       		 equalizer[2]= dis;
                flags+= std::to_string(dis);
            }
        }
        flags+= " ";
        
	if(v_data[3]>15)
        {
            if(v_data[4]==1){
		equalizer[3] = 10;
            }else{ 
		equalizer[3] = -10;              
            }
        }
        
        if(v_data[3]>YAW_RANGE )
        {
            if(v_data[4]==1){
            	equalizer[3] = -4;
            	flags+="4";
                flags = "rc 10 -5 0 10";
		equalizer[0] = 10;
		equalizer[1] = -5;
		equalizer[2] = 0;
		equalizer[3] = 10;
            }else{ 
                equalizer[3] = 4;
                flags+="4";
                flags = "rc -10 -5 0 -10";
		equalizer[0] = -10;
		equalizer[1] = -5;
		equalizer[2] = 0;
		equalizer[3] = -10;              
            }
        }
        
    }else{
    	flags = "rc 0 0 0 0";
    }
    return equalizer;

}


 

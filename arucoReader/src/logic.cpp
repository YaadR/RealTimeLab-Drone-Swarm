#include "logic.h"


#define MAX_DIST 1.2;
#define MIN_DIST 0.8;
#define MAX_LEFT_RIGHT 0.4;
#define MIN_LEFT_RIGHT 0.2;
#define UP_DOWN_RANGE 0.15;
#define YAW_RANGE 0.1;
#define ROTATE_RANGE 0.1;


 vector<double> datAruco(aruco &detector){
    
    vector<double> dat_vec;
    dat_vec.push_back((double)detector.forward);
    dat_vec.push_back((double)detector.rightLeft);
    dat_vec.push_back((double)detector.upDown);
    dat_vec.push_back((double)detector.leftOverAngle.first);
    dat_vec.push_back((double)detector.leftOverAngle.second);
    dat_vec.push_back((double)detector.idr);
    dat_vec.push_back((double)detector.ifArucoExist);

    return dat_vec;
    
}

void move_to(vector<double> v_data){
/*
v_data[0] = forward
v_data[1] = rightLeft
v_data[2] = upDown
v_data[3] = leftOverAngle.first
v_data[4] = leftOverAngle.second
v_data[5] = idr
v_data[6] = ifArucoExist
*/

    if(v_data[6]){
        if(v_data[0]<MIN_DIST || v_data[0]>MAX_DIST){
            if(v_data[0]<MIN_DIST){
                std::cout << "Move Backward" << std::endl;
            }else{
                std::cout << "Move Forward" << std::endl;
            }
        }

        if(abs(v_data[1])>MAX_LEFT_RIGHT||abs(v_data[1])<MIN_LEFT_RIGHT){
            switch(v_data[5]%2){        // Even -> 0 -> Left || Odd -> 1 -> Right
                case 0:
                    if(v_data[1]<-MAX_LEFT_RIGHT){
                        std::cout << "Move Right" << std::endl;
                    }else{
                        std::cout << "Move Left" << std::endl;
                    }                  
                    break;
                case 1:
                    if(v_data[1]<MIN_LEFT_RIGHT){
                        std::cout << "Move Right" << std::endl;
                    }else{
                        std::cout << "Move Left" << std::endl;
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
            }else{
                std::cout << "Move Up" << std::endl;
            }
        }

        if(abs(v_data[3])>YAW_RANGE)
        {
            if(v_data[3]>YAW_RANGE){
                std::cout << "Yaw right" << std::endl;
            }else{
                std::cout << "Yaw left" << std::endl;
            }
        }

        if(abs(v_data[4])>ROTATE_RANGE)
        {
            if(v_data[3]>ROTATE_RANGE){
                std::cout << "Rotate right" << std::endl;
            }else{
                std::cout << "Rotate left" << std::endl;
            }
        }

    }
}

int main(){

    while(true){
        aruco::detector(aruco::aruce());
        move_to(datAruco(detector));
    }
    return 0;
}





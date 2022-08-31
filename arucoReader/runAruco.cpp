//
// Created by tzuk on 6/6/22.
//
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "aruco.h"
#include "drone.h"
#include <thread>

//TELLO-F1F5FB

/*@Y*/

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <ctello.h>
#include <string.h>
/*@Ye*/


/*Tello part*/
//std::shared_ptr<cv::VideoCapture> capture;
//std::shared_ptr<cv::Mat> frame;
//std::shared_ptr<bool> stop;
/*
void videoDroneThread(std::string &videoPath){
    capture = std::make_shared<cv::VideoCapture>(videoPath);
    frame = std::make_shared<cv::Mat>();
    while(!*stop){
        capture->read(*frame);
    }
    capture->release();
}

void keepAlive(ctello::Tello &tello){
   	 while(!*stop){
		tello.SendCommandWithResponse("battery?");
        sleep(10);
	}
}
*/
char keepBreathing(char sign, ctello::Tello &tello){ // Added this function inteed of keepAlive(ctello::Tello &tello) for simplicity
	std::string rc = "rc +0 +0 +5 +0";
	rc[9]=sign;
	tello.SendCommand(rc);
	return sign=='+'?'-':'+';
	
}
void theEqualizer(int ac[], int* eq, int round){
	ac[0] += eq[0];
	ac[1] += eq[1];
	ac[2] += eq[2];
	ac[3] += eq[3];
	if(round%10==0){
		ac[0] = ac[0]/10;
		ac[1] = ac[1]/10;
		ac[2] = ac[2]/10;
		ac[3] = ac[3]/10;
	}
	
		
}

std::string theStringer(int* arr){
	std::string str = "rc  00  00  00  00";
	std::string strd = "00";
	if(arr[0]<0){
		strd = std::to_string(abs(arr[0]));
                str[4]= strd[0];
                str[5]= strd[1];
                str[3]='-';
	}else{
		strd = std::to_string(arr[0]);
                str[4]= strd[0];
                str[5]= strd[1];
	}
	
	if(arr[1]<0){
		strd = std::to_string(abs(arr[1]));
                str[8]= strd[0];
                str[9]= strd[1];
                str[7]='-';
	}else{
		strd = std::to_string(arr[1]);
                str[8]= strd[0];
                str[9]= strd[1];
	}
	
	if(arr[2]<0){
		strd = std::to_string(abs(arr[2]));
       		 str[11]='-';
                str[12]= strd[0];
                str[13]= strd[1];
	}else{
		strd = std::to_string(arr[2]);
                str[12]= strd[0];
                str[13]= strd[1];
	}
	
	if(arr[3]<0){
		strd = std::to_string(abs(arr[3]));
                        str[15]='-';
                	str[16]=strd[0];
                	str[17]=strd[1];
	}else{
		strd = std::to_string(arr[3]);
        	str[16]=strd[0];
        	str[17]=strd[1];;
	}
	 
	 return str;
}
/*Tello part end*/

void runAruco(aruco &detector, drone *myDrone, ctello::Tello& tello){
	
        char sign = '+';
        int staller  = 0;
        int* equalizer;
	int accumulate[4] = {0};
        
        while(detector.ifArucoExist == 0){
        	std::cout << "waiting for aruco" << std::endl;        	
	}
	//sleep(0.05);
	if(detector.rightLeft < 0.3)
        	myDrone->setRightOrLeft(0); // left drone
        else
        	myDrone->setRightOrLeft(1); // right drone
        	
    while(true){
	
        myDrone->addInfo(detector);
        //tello.SendCommand(myDrone->move_drone());

        staller++;
        //std::string movingCommand= myDrone->move_drone();

	equalizer = myDrone->move_drone();
	
	theEqualizer(accumulate, equalizer, staller);
	
        if(staller%10==0){
	        std::string movingCommand = theStringer(accumulate);  
	        tello.SendCommand(movingCommand);
	        accumulate[0] = 0;
	        accumulate[1] = 0;
	        accumulate[2] = 0;
	        accumulate[3] = 0;
        	//tello.SendCommand("rc 0 5 0 0");
        	std::cout << "passing: " <<movingCommand << std::endl;
        }
        
       sleep(0.1);
        

        //sign = keepBreathing(sign,std::ref(tello));

        
    
}
}


/*cv::imshow("aruco", *frame);
        cv::waitKey(1);*/
int main(){
    std::ifstream programData("../config.json");

   
	//system("v4l2 -d /dev/video0 focus_auto=0");

 
    drone myDrone;
    
    nlohmann::json data;
    programData >> data;
    programData.close();
    std::string droneName = data["DroneName"];
    std::string commandString = "nmcli c up " + droneName;
    const char *command = commandString.c_str();
    system(command);
    ctello::Tello tello;
    tello.SendCommandWithResponse("streamon");
    std::string videoPath = data["cameraString"];
    std::string yamlCalibrationPath = data["yamlCalibrationPath"];
    bool isCameraString = data["isCameraString"];
    float currentMarkerSize = data["currentMarkerSize"];
    
    if (isCameraString){
        std::string cameraString = data["cameraString"];
        aruco detector(yamlCalibrationPath,cameraString,currentMarkerSize);
    	//tello.SendCommandWithResponse("takeoff");
    	runAruco(detector,&myDrone,tello);
    }else{
        int cameraPort = data["cameraPort"];       
        aruco detector(yamlCalibrationPath,cameraPort,currentMarkerSize);
    	runAruco(detector, &myDrone ,tello);

    }
    
    /*
    std::thread t_vid = std::thread(videoDroneThread,std::ref(videoPath));
    std::thread t_heartbeat = std::thread(keepAlive, std::ref(tello));
    int waitKey = 0;
    /td::cout << "frame size" << *frame->size << std::endl;
    for (int i{0}; i < 100; i++) {
	    cv::Mat frameCopy = *frame;
	cv::imshow("Image", frameCopy);
	cv::waitKey(1);
    }
    *stop = true;
    t_vid.join();
    t_heartbeat.join();*/
    tello.SendCommandWithResponse("land");
    cv::destroyAllWindows();
	
 	
    	
    return 0;
}



        /*cv::imshow("aruco", *frame);
        cv::waitKey(1);*/
    	/*std::thread th1(tzukArucoReaderRun, &ourDrone);
    	std::thread th2(run, &ourDrone);*/
    	
       /*std::cout 
       << "forward: " << detector.forward 
        << " right left: " << detector.rightLeft 
        << " updown: " << detector.upDown
        << " angle: " << detector.leftOverAngle.first 
        << " clockwise: " << detector.leftOverAngle.second 
        << " id: " << detector.idr 
        << " read Aruco: " << detector.ifArucoExist
        << " right or left: " << myDrone->getRightOrLeft()
        << std::endl;/**/
        
        
        

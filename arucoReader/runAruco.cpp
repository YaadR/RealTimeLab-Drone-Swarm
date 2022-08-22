//
// Created by tzuk on 6/6/22.
//
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "aruco.h"
#include "drone.h"
#include <thread>

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
std::shared_ptr<cv::VideoCapture> capture;
std::shared_ptr<cv::Mat> frame;
std::shared_ptr<bool> stop;
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

char keepBreathing(char sign, ctello::Tello &tello){ // Added this function inteed of keepAlive(ctello::Tello &tello) for simplicity
	std::string rc = "rc +0 +0 +5 +0";
	rc[9]=sign;
	tello.SendCommand(rc);
	std::cout << rc << std::endl;
	return sign=='+'?'-':'+';
	
}
/*Tello part end*/

void runAruco(aruco &detector, drone *myDrone, ctello::Tello &tello){
	
        char sign = '+';
        
        while(detector.ifArucoExist != 1){
        	std::cout << "waiting for aruco" << std::endl;
        	sign = keepBreathing(sign,std::ref(tello));
        	
	}
	sleep(1);
	if(detector.rightLeft < 0.3)
        	myDrone->setRightOrLeft(0); // left drone
        else
        	myDrone->setRightOrLeft(1); // right drone
    while(true){
	
        myDrone->addInfo(detector);
        tello.SendCommand(myDrone->move_drone());
        sign = keepBreathing(sign,std::ref(tello));
        //std::cout << myDrone->getRightOrLeft() << std::endl;

        
        
    }
}/*
void tzukArucoReaderRun(drone *myDrone){
    std::ifstream programData("../config.json");
    nlohmann::json data;
    programData >> data;
    programData.close();
    std::string yamlCalibrationPath = data["yamlCalibrationPath"];
    bool isCameraString = data["isCameraString"];
    float currentMarkerSize = data["currentMarkerSize"];
    if (isCameraString){
        std::string cameraString = data["cameraString"];
        aruco detector(yamlCalibrationPath,cameraString,currentMarkerSize);
    	runAruco(detector, myDrone);
    }else{
        int cameraPort = data["cameraPort"];
        aruco detector(yamlCalibrationPath,cameraPort,currentMarkerSize);
    	runAruco(detector, myDrone);
    }
}*/
/*void run(drone *myDrone){
	myDrone->runDrone();
}*/

/*cv::imshow("aruco", *frame);
        cv::waitKey(1);*/
int main(){
	//system("v4l2 -d /dev/video0 focus_auto=0");
	
	
        //std::string settingPath = "../config.json";
        
	drone myDrone;
	std::ifstream programData("../config.json");
    nlohmann::json data;
    programData >> data;
    programData.close();
    std::string droneName = data["DroneName"];
    std::string commandString = "nmcli c up " + droneName;
    const char *command = commandString.c_str();
    system(command);
    ctello::Tello tello;
    //tello.SendCommandWithResponse("takeoff");  
    tello.SendCommandWithResponse("streamon");
    std::string videoPath = data["cameraString"];

        std::string yamlCalibrationPath = data["yamlCalibrationPath"];
    bool isCameraString = data["isCameraString"];
    float currentMarkerSize = data["currentMarkerSize"];
    if (isCameraString){
        std::string cameraString = data["cameraString"];
        aruco detector(yamlCalibrationPath,cameraString,currentMarkerSize);
    	runAruco(detector, &myDrone,std::ref(tello));
    }else{
        int cameraPort = data["cameraPort"];
        aruco detector(yamlCalibrationPath,cameraPort,currentMarkerSize);
    	runAruco(detector, &myDrone,std::ref(tello));
    }
    
    
    stop = std::make_shared<bool>(false);
    std::thread t_vid = std::thread(videoDroneThread,std::ref(videoPath));
    std::thread t_heartbeat = std::thread(keepAlive, std::ref(tello));
    int waitKey = 0;
    std::cout << "frame size" << *frame->size << std::endl;
    for (int i{0}; i < 100; i++) {
	    cv::Mat frameCopy = *frame;
	cv::imshow("Image", frameCopy);
	cv::waitKey(1);
    }
    *stop = true;
    t_vid.join();
    t_heartbeat.join();
    //tello.SendCommandWithResponse("land");
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
        
        
        

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

int AVRAGE = 5;
float SLEEP =1000;
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
	if(round%AVRAGE==0){
		ac[0] = ac[0]/AVRAGE;
		ac[1] = ac[1]/AVRAGE;
		ac[2] = ac[2]/AVRAGE;
		ac[3] = ac[3]/AVRAGE;
	}
	
		
}

//std::string theStringer(int* arr){
void theStringer(int* arr,ctello::Tello& tello){
	std::string str = "rc ";
        str+= std::to_string(arr[0]);
	str+=" ";
        str+= std::to_string(arr[1]);
	str+=" ";
        str+= std::to_string(arr[2]);
	str+=" ";
        str+= std::to_string(arr[3]);

	 
	 std::cout << "passing: " <<str << std::endl;
	 tello.SendCommand(str);
}
/*Tello part end*/

void acquiring(aruco &detector, ctello::Tello& tello){
	int count = 100;
	
	if(detector.ifArucoExist == 0){
		
		while(detector.ifArucoExist != 1){

			std::cout << " can't find aRuco" << std::endl;
			tello.SendCommand("rc 0 0 0 40");
			usleep(500);
			count--;

		}
	}
	if(count == 0)
		//tello.SendCommand("land");
	return;
}
void runAruco(aruco &detector, drone *myDrone, ctello::Tello& tello){
	
        char sign = '+';
        int staller  = 0;
        int* equalizer;
	int accumulate[4] = {0};
        
        while(detector.ifArucoExist == 0){
        	std::cout << "waiting for aruco" << std::endl;        	
	}
	
	tello.SendCommand("rc 0 0 20 0");
	sleep(2);
	/*if(detector.rightLeft > 0.2)
        	myDrone->setRightOrLeft(0); // left drone
        else
        	myDrone->setRightOrLeft(1); // right drone*/
        	
       myDrone->setRightOrLeft(1);
        	
       	int dontsee = 0;
    while(true){

        myDrone->addInfo(detector);
        //tello.SendCommand(myDrone->move_drone());
	
        staller++;
        //std::string movingCommand= myDrone->move_drone();

	equalizer = myDrone->move_drone();
	if(equalizer[0] == -1 && equalizer[1] == -1 && equalizer[2] == -1 && equalizer[3] == -1){
		dontsee++;
		if(dontsee >= 1000){
			acquiring(detector, tello);
			dontsee = 0;
		}
		equalizer[0] == 0;
		equalizer[1] == 0;
		equalizer[2] == 0;
		equalizer[3] == 0;
		std::cout << dontsee << std::endl;
		tello.SendCommand("rc 0 0 0 0");
	}
	else{
		dontsee = 0;
		theEqualizer(accumulate, equalizer, staller);	
	        if(staller%AVRAGE==0){
		        //std::string movingCommand = theStringer(accumulate);  
		        theStringer(accumulate,tello);	        
		        accumulate[0] = 0;
		        accumulate[1] = 0;
		        accumulate[2] = 0;
		        accumulate[3] = 0;
        		

        	}
		if(staller%10==0){
			std::cout << std::endl << std::endl
	       		<< "forward: " << detector.forward 
			<< " right left: " << detector.rightLeft 
			<< " updown: " << detector.upDown
			<< " right or left: " << myDrone->getRightOrLeft()
			<< std::endl << std::endl;}
       }
       usleep(SLEEP);
       //tello.SendCommand("rc 0 0 0 0");

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
    	tello.SendCommandWithResponse("takeoff");
    	runAruco(detector,&myDrone,tello);
    }else{
        int cameraPort = data["cameraPort"];       
        aruco detector(yamlCalibrationPath,cameraPort,currentMarkerSize);
        tello.SendCommandWithResponse("takeoff");
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
        
        
        

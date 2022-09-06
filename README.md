# RealTime Lab - Drone Swarm


**University of Haifa - RealTime Lab**

### Authors:

* [Almog Hadad](https://github.com/AlmogHadad)
* [Daniel Ramrazker](https://github.com/danielramraz)
* [Yaad Rebhun](https://github.com/YaadR)

## Project description:
Implementing logic for drone swarm, to aquire and follow aruco, and maintain flight structure , with minimal communication

## Run Me:
IDE: **Raspberry.pi**
### Installations required:
```
install cmake
install tello
```
### Libraries used:
1. opencv :
   * `opencv2`- used to create the aruco reader.
2. ctello:
   * `ctello` - used to control the drone.
3. thread:
   * `thread` - creating multithreaded program.

### How to run:  
* download the project to <"some path">.
* make sure your drone connect to your computer with wifi.
* in command line:
```
cd <some path>
make
./runAruco
```

### Basic interupts:
#### Quit program:
You can quit by pressing "ctr + c", the drone will land in his place.

## Algorithem & Math:
#### 1. Acquiring leader
  * The drone detect leader aruco and set his own position in the swarm according to relative starting position.
  * The drone use the Aruco reader to detect the leader, or any other drone.
#### 2. Detecting leader movement
  * The aruco reader detect the leader movement in 4 demantions:
    ##### * up down
    ##### * left right
    ##### * forward
    ##### * yaw
  * The aruco send the data to drone for calculations.
#### 3. Bounding box
  * Frames the boundaries of the detected leader, calculates the leader movement in 4 dementions into one command.
#### 4. Drone movement logic:
  * Controling commands frequancy and motor power with averaging data, so the drone movement became smoth.
#### 5. Lost leader:
  * In case of losing the leader sight the drone will hold in place and make 360 look around, to check for reconnection with leader.

## Objectives towards finalized product:
* ???????????????????????????????

*
*
*
*
*
*
*
----------------------------------------------------------------------------------------------------------------




//Y@ not sure if actually needed.. 
Project files: 
1. `PoseEstimationModule.py` - Identify an object and find landmarks 
2. `PoseProject.py` - Capture image, detect object, and calculate object motion


## Run Me:
IDE: **Raspberry.pi**
### Installations:

//@Y also not sure what to write here...
```
pip install opencv-python
pip install mediapipe 
pip install numpy
```

To run the code on the Tello drone, install [tello directory.](https://github.com/damiafuentes/DJITelloPy)

### Libraries:
1. PoseEstimationModule.py :
   * `Mediapipe`- ML solution for real-time object detection, used for pedestrian detection
   * `time` - Synchronizes and tracks real-time processes
   * `Numpy` - Performs matrix operations on an image
2. PoseProject.py:
   * `argv from sys` - Passes arguments from external devices
   * `opencv /cv2` - Library of programming functions mainly aimed at real-time CV
   * `time` - Synchronizes and tracks real-time processes
   * `PoseEstimationModule` – Our module
   * `Numpy` - Performs matrix operations on an image
   * `Datetime` – Records implementation
   * `Tello form djitellopy` – Tello drone API
### Arguments:
The program recive 2 input argoments
 * `argv[1]` – Captures input. `0`= Integrated camera; `1`= Drone; `<file_path>`= Reads video file
 * `argv[2]`- Draws symbols over the input. `True`/`False`

### Basic interupts:
#### Quit program:
You can quit by pressing `Esc` button. If the program is at ***drone mode*** the drone will land with the execution of this command
#### Recording:
The program has video recording capabilities. Given the recording command, a red circle mark appears in the upper right corner of the window
* `R` - start recording
* `S` - stop recording

## Modes:
1. ***Integrated camera mode*** - `argv[1]='0'`
* ![Integrated camera mode](MarkdownFiles/integrated_camera.gif)

2. ***Drone mode*** - `argv[1]='1'`
* ![Drone mode2](MarkdownFiles/drone_flight2.gif)
* ![Drone mode1](MarkdownFiles/drone_flight1.gif)


3. ***Read video/image file*** - `argv[1]=<file_path>`
* ![Drone mode1](MarkdownFiles/video_file.gif)

## Math & Logic operations:
#### 1. Rescale frame
Resize the image according to the given percentage ratio
#### 2. Hotzones
Drawing perpendicular lines that maintain the pre-borders of the frame, relative to acquired object or given values.
* The <span style="color:red">***red***</span> lines - marks an anomaly of the object from the allowed regions, positioned at one body-width from the edges of the frame.
* The <span style="color:yellow">***yellow***</span> lines - marks the place where the object is suppose to move, if the red line is crossed, positioned at two body-widths away from the frame edges.
  * The lines will collide at the frame center, under no circumstances will the yellow lines overlap each other

![HotZones](/MarkdownFiles/hotZones.png)

#### 3. Bounding box
Frames the boundaries of the detected object, calculates the maximum length between the figure's shoulders and the figure's waist.
#### 4. From detection to acquirement
In order to acquire the character, all parts of the image that do not contain the identified object are zero-padded.
At object detection, a counter ('adj') counts frames. As it finds an object it crops a relative window, sized 1.8 times the figure bounding box size, zero-pads the rest of the frame,
and 'feeds' the detection algorithm with the cropped frame at subsequent
cycle.

![croped image](/MarkdownFiles/crop_image.png)

#### 5. Drone movement logic:
  * front – object to frame ratio. front >0.4 – move backward, front<0.2 - move
  forward
  * yaw – derive logic from Hotzones – while red line is crossed, rotate towards the
  frame center until yellow line is crossed back to approved zone
  * isUp – solution for Tello drone properties – gives minor up and down
  movement so the drone remains active (the Tello drone aborts after 15s of not
  receiving any orders)
  
## Objectives towards finalized product:
* Modularity - Option to run on a number of different devices
* Loading and running over raspberry pi
* Scalability - Supports the identification and acquires multiple objects simultaneously
* Robustness - resistance to extreme cases
* MediaPipe is Google's open source directory and therefore it is not legal to market the
product as is, at least in principle

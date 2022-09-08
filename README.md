# RealTime Lab - Drone Swarm


**University of Haifa - RealTime Lab**

### Authors:

* [Almog Hadad](https://github.com/AlmogHadad)
* [Daniel Ramrazker](https://github.com/danielramraz)
* [Yaad Rebhun](https://github.com/YaadR)

## Project description:
#### Implementing:
* Aruco detection and position data
* Drone movment & swarm structure
* Robust flight structure, with minimal communication


![Swarm video](Vid/swarmgif.gif)
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
cd <some path>/arucoReader/build
make
./runAruco
```

### Basic interupts:
#### Quit program:
You can quit by pressing "ctr + c", the drone will land in his place.

## Algorithem & Math:
#### 1. Acquiring leader
  * The drone detect leader aruco and set his own position in the swarm according to relative starting position.
  * The drone use the Aruco reader to detect the leader.
  
  
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
  * In case of losing the leader sight the drone will hover in place and make 360 look around, to check for reconnection with leader.


*
*
*
*
*
*
*
----------------------------------------------------------------------------------------------------------------


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


Frames the boundaries of the detected object, calculates the maximum length between the figure's shoulders and the figure's waist.
#### 4. From detection to acquirement
In order to acquire the character, all parts of the image that do not contain the identified object are zero-padded.
At object detection, a counter ('adj') counts frames. As it finds an object it crops a relative window, sized 1.8 times the figure bounding box size, zero-pads the rest of the frame,
and 'feeds' the detection algorithm with the cropped frame at subsequent
cycle.

![croped image](/MarkdownFiles/crop_image.png)

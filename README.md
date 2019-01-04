### lidar_obstacle_avoidance

A simple tool to split the lidar data into multiple areas, just like multiple ultrasonic sensors

>Author: Sunshengjin @MindPointEye
1012116832@qq.com

---

####  Why made it 
>Because the ultrasonic sensor is not so reliable, and in the mobile robot or some area protection tasks, it is necessary to detect and warn the obstacles in a specific range or area, so i made this small tool and used the reliable data of the lidar to be divided into multiple programmable areas to detect obstacles.

#### How to use it 
>Recommended system environment: **Ubuntu16.04 + ROS Kinetic**
>+ **Note**:It not rely heavily on a specific system environment, It can also run smoothly on other system versions, Such as **Ubuntu14.04 + ROS Indigo**.
##### Step 1
> Copy this package into your {ros_workspace}/src
```
    $ cd catkin_workspace/src
    $ git clone https://github.com/Sunshengjin/lidar_obstacle_avoidance.git
    $ cd ../
```
##### Step 2
>Compile your ros workspace
```
    $ catkin_make
    $ rospack profile
```
##### Step 3
>  Please run your *lidar driver node*
>   **Note**:Make sure your lidar publish topic is as seem as the topic setting which in the demo.launch.  
>  Then you can run this tool:
```
    $ roslaunch lidar_obstacle_avoidance demo.launch
```






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

#### Configuration 
> This tools just subscribe the *lidar scan* topic ,and it will publish several topic based on  the number you set the areas.
The configuration file is *{ros_workspace}/src/lidar_obstacle_avoidance/launch/demo.launch*.

**demo.launch**
 ```

    <arg name="Pi" value="3.1415926"/>
    <node name="loa_node" pkg="lidar_obstacle_avoidance" type="loa_node" output="screen" respawn="true">
       
        <param name="scan_topic" type="string" value="/scan"/>

        <param name="num_of_area" type="int" value="5"/>


        <param name="area_1_min_ang"    type="double" value="-90"/>
        <param name="area_1_max_ang"      type="double" value="-60"/>
        <param name="area_1_max_range"    type="double" value="3"/>
        <param name="area_1_min_range"      type="double" value="1"/>
        <param name="area_1_filter_size"  type="int"    value="3"/>

        <param name="area_2_min_ang"    type="double" value="-50"/>
        <param name="area_2_max_ang"      type="double" value="-20"/>
        <param name="area_2_max_range"    type="double" value="3"/>
        <param name="area_2_min_range"      type="double" value="1"/>
        <param name="area_2_filter_size"  type="int"    value="3"/>


        <param name="area_3_min_ang"    type="double" value="-10"/>
        <param name="area_3_max_ang"      type="double" value="20"/>
        <param name="area_3_max_range"    type="double" value="3"/>
        <param name="area_3_min_range"      type="double" value="1"/>
        <param name="area_3_filter_size"  type="int"    value="3"/>
        
        <param name="area_4_min_ang"    type="double" value="30"/>
        <param name="area_4_max_ang"      type="double" value="50"/>
        <param name="area_4_max_range"    type="double" value="3"/>
        <param name="area_4_min_range"      type="double" value="1"/>
        <param name="area_4_filter_size"  type="int"    value="3"/>

        <param name="area_5_min_ang"    type="double" value="60"/>
        <param name="area_5_max_ang"      type="double" value="90"/>
        <param name="area_5_max_range"    type="double" value="3"/>
        <param name="area_5_min_range"      type="double" value="1"/>
        <param name="area_5_filter_size"  type="int"    value="3"/>
    </node>

    <!--ang to rad :rad =  PI/180 * ang -->
    <!--How to calculat the Yaw set -->
    <!-- Yaw = (max_ang + min_ang)/2 * (PI / 180) -->
    <node pkg="tf" type="static_transform_publisher" name="lidar_to_range1" args="0.0 0.0 0.0 -1.309 0.0 0.0 /laser /range1 40"/>
    <node pkg="tf" type="static_transform_publisher" name="lidar_to_range2" args="0.0 0.0 0.0 -0.611 0.0 0.0 /laser /range2 40"/>
    <node pkg="tf" type="static_transform_publisher" name="lidar_to_range3" args="0.0 0.0 0.0 0.0872 0.0 0.0 /laser /range3 40"/>
    <node pkg="tf" type="static_transform_publisher" name="lidar_to_range4" args="0.0 0.0 0.0 0.698 0.0 0.0 /laser /range4 40"/>
    <node pkg="tf" type="static_transform_publisher" name="lidar_to_range5" args="0.0 0.0 0.0 1.309 0.0 0.0 /laser /range5 40"/>
</launch>

 ```
If you want to change the configuration to what you need,just follow the steps below:

**Step 1**
> Make sure your lidar publish topic is as seem as the topic setting which in the demo.launch.  
 ```
        <param name="scan_topic" type="string" value="/scan"/>
 ```
 **Step 2**
 > Set the number of the defence area.
 ```
        <param name="num_of_area" type="int" value="5"/>
 ```

 **Step 3**
 > Set parameters for each area.
 ```
example:
        <param name="area_1_min_ang"    type="double" value="-90"/>
        <param name="area_1_max_ang"      type="double" value="-60"/>
        <param name="area_1_max_range"    type="double" value="3"/>
        <param name="area_1_min_range"      type="double" value="1"/>
        <param name="area_1_filter_size"  type="int"    value="3"/>
 ```
 + **min_ang** : start angle of the area [ang]
 - **max_ang** : end angle of the area [ang]
 + **max_range** : maximum range value [m]
 - **min_range** : minimum range value [m]
 + **filter_size** : The size of the data filter.*If the number of obstacle sampling points is less than the set value,this data will be ignored.*

 **Step 4**
 > Set the transform tree for each area
 ```
 example:
    <node pkg="tf" type="static_transform_publisher" name="lidar_to_range1" args="0.0 0.0 0.0 -1.309 0.0 0.0 /laser /range1 40"/>

 ```
 >**Note**:you only need to set the 4th parameter of the *args* tags,you need to manually calculate the value of this parameter yourself.
 The calculation formula is {Yaw = (max_ang + min_ang)/2 * (PI / 180)}

 **Step 5**
 >Now you can run this tools ,and you can subscribe the range topic : */rangeX* (X is which of the area range data you want to get).






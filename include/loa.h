#ifndef LOA_H
#define LOA_H

#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>
#include <std_msgs/Bool.h>
#include "ros/ros.h"
#include <vector>
//#include "lidar_obstacle_avoidance/loa_msgs.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>


using namespace std;


#define PI  3.141592653
#define myabs(x) (x)>0? (x) : (-(x))
typedef struct loa_data_struct
{
    double   min_ang;
    double   max_ang;
    double   max_range;
    double   min_range;
    std::vector<double> alert_area;
    int      filter_size;
    bool     warning;
    double   nearest_obstacle_distance;
   
}loa_data;

class LOA_CORE
{
    public:
        LOA_CORE();
        ~LOA_CORE();


        void import_config(loa_data& area);    //导入防区
        void start(void);                         //开始监控防区


    private:
        sensor_msgs::LaserScan scan_;  //储存接收到的雷达数据
        std::vector<loa_data> area_group_;
        
        sensor_msgs::Range range_temp_;
        std::vector<ros::Publisher>  range_pub_group_;
        

        //lidar_obstacle_avoidance::loa_msgs  obs_flag_;

        int number_of_area_;

        loa_data area_temp_;
        std::stringstream oss_temp;
      /*  loa_data area_1;
        loa_data area_2;
        loa_data area_3;
        */

        ros::Subscriber lidar_sub_;     //监听雷达数据
        ros::Publisher  obs_flag_pub_;  //发布标志位

        ros::Publisher  range_pub_temp_;

        void filter(void);            //根据设置的滤波器大小，过滤选区内的雷达数据
        void fill_scan_to_zone(void);  //根据设置的防区范围，填充雷达数据
        void scan_callback(const sensor_msgs::LaserScan::ConstPtr &scan);
        void publish_result(void);
        void find_the_nearest_obs(void);


};






#endif
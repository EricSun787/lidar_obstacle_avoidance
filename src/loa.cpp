#include "loa.h"

LOA_CORE::LOA_CORE()
{
    ros::NodeHandle nh;
    ros::NodeHandle nh_p("~");
    ros::Rate rate(30);

    

    std::string scan_topic_;
    //std_msgs::Bool init_flag_;
    //init_flag_.data = false;




    nh_p.param<std::string>("scan_topic",scan_topic_,"/scan_1");
    nh_p.param<int>("num_of_area",number_of_area_,3);

    std::cout << "Got " << number_of_area_ << " Area Setting" << std::endl;

    for(int temp = 1; temp <= number_of_area_; temp ++)
    {
        oss_temp << "area_" << temp << "_min_ang";
        nh_p.param<double>(oss_temp.str(),area_temp_.min_ang,0.0);
        std::cout << "get " << oss_temp.str() << ": " << area_temp_.min_ang << std::endl;
        oss_temp.str("");

        oss_temp << "area_" << temp << "_max_ang";
        nh_p.param<double>(oss_temp.str(),area_temp_.max_ang,0.0);
        std::cout << "get " << oss_temp.str() << ": " << area_temp_.max_ang << std::endl;
        oss_temp.str("");

        oss_temp << "area_" << temp << "_max_range";
        nh_p.param<double>(oss_temp.str(),area_temp_.max_range,0.0);
        std::cout << "get " << oss_temp.str() << ": " << area_temp_.max_range << std::endl;
        oss_temp.str("");

        oss_temp << "area_" << temp << "_min_range";
        nh_p.param<double>(oss_temp.str(),area_temp_.min_range,0.0);
        std::cout << "get " << oss_temp.str() << ": " << area_temp_.min_range << std::endl;
        oss_temp.str("");

        oss_temp << "area_" << temp << "_filter_size";
        nh_p.param<int>(oss_temp.str(),area_temp_.filter_size,0);
        std::cout << "get " << oss_temp.str() << ": " << area_temp_.filter_size << std::endl;
        oss_temp.str("");

        import_config(area_temp_);

        oss_temp << "range" << temp ;
        range_pub_temp_ = nh.advertise<sensor_msgs::Range>(oss_temp.str(),10);
        range_pub_group_.push_back(range_pub_temp_);
        oss_temp.str("");
    }
    std::cout << "We have " <<range_pub_group_.size() << " range publisher"<< std::endl;

    lidar_sub_ = nh.subscribe<sensor_msgs::LaserScan>(scan_topic_,1,boost::bind(&LOA_CORE::scan_callback,this,_1));
    //obs_flag_pub_ = nh.advertise<lidar_obstacle_avoidance::loa_msgs>("/loa",1);

    //obs_flag_.zone.resize(number_of_area_);
}
LOA_CORE::~LOA_CORE()
{

}

void LOA_CORE::import_config(loa_data& area)
{
    if(area.max_range != 0)
    {
        //ROS_INFO("Loading %d Denfence Area",area_group_.size()+1);
        std::cout << "Loading " << area_group_.size() +1 << " Denfence Area" << std::endl;
        area_group_.push_back(area);
    }
}

void LOA_CORE::scan_callback(const sensor_msgs::LaserScanConstPtr &scan)
{
    scan_ = *scan;
    fill_scan_to_zone();
    filter();
    find_the_nearest_obs();
    publish_result();
}

void LOA_CORE::fill_scan_to_zone(void)
{

    for(int i = 0; i < area_group_.size(); i++)
    {
        std::vector<double>().swap(area_group_[i].alert_area);
       for(int j = (int)((area_group_[i].min_ang * (PI / 180) - scan_.angle_min) / scan_.angle_increment);j < (int)( ((area_group_[i].max_ang * (PI / 180)) - scan_.angle_min) / scan_.angle_increment ) ; j++)
            {
                area_group_[i].alert_area.push_back(scan_.ranges[j]);
            }
    }
}
void LOA_CORE::filter(void)
{
    for(int i = 0; i < area_group_.size(); i++)
    {
        int k = 0;
        for(int j = 0; j < area_group_[i].alert_area.size(); j++)
            {
                if(area_group_[i].alert_area[j] <= area_group_[i].max_range && area_group_[i].alert_area[j] >= area_group_[i].min_range)
                    {
                        k++;
                        if(k >= area_group_[i].filter_size)
                        {
                            area_group_[i].warning = true;
                            break;
                        }
                    }
                else
                {
                    k = 0;
                }
                area_group_[i].warning = false;
            }
    }
}
void LOA_CORE::start(void)
{
    ros::spin();
}
void LOA_CORE::publish_result(void)
{

    for(int i = 0; i < area_group_.size();i++)
    {
        range_temp_.radiation_type = sensor_msgs::Range::INFRARED;

        oss_temp << "/range" << i+1 ;
        range_temp_.header.frame_id = oss_temp.str();
        oss_temp.str("");
        range_temp_.header.stamp = ros::Time::now();
        range_temp_.field_of_view = (area_group_[i].max_ang*(PI/180)) - (area_group_[i].min_ang*(PI/180));
        range_temp_.min_range = area_group_[i].min_range;
        range_temp_.max_range = area_group_[i].max_range; 
        range_temp_.range = area_group_[i].nearest_obstacle_distance;
        range_pub_group_[i].publish(range_temp_);
    }
    //for(vector<int>::size_type i = 0; i != area_group_.size(); ++i)
/*    for(int i = 0; i < area_group_.size(); i++)
    {
        if(area_group_[i].warning == true)
            obs_flag_.zone[i].data = true;
        else
            obs_flag_.zone[i].data = false;
    }
    obs_flag_.header.stamp = ros::Time::now();
    obs_flag_.header.frame_id = "laser";
    obs_flag_pub_.publish(obs_flag_);
    */
}

void LOA_CORE::find_the_nearest_obs(void)
{
    for(int i = 0; i < area_group_.size(); i++)
    {
        area_group_[i].nearest_obstacle_distance = area_group_[i].max_range;
        for(int j = 0; j < area_group_[i].alert_area.size(); j++)
        {
                area_group_[i].nearest_obstacle_distance = area_group_[i].nearest_obstacle_distance > area_group_[i].alert_area[j] ? area_group_[i].alert_area[j] : area_group_[i].nearest_obstacle_distance;
                if(area_group_[i].nearest_obstacle_distance > area_group_[i].max_range)
                    area_group_[i].nearest_obstacle_distance = area_group_[i].max_range;
                if(area_group_[i].nearest_obstacle_distance < area_group_[i].min_range)
                    area_group_[i].nearest_obstacle_distance = area_group_[i].min_range;
        }
    }
}

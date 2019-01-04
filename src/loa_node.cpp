#include <loa.h>

int main(int argc,char** argv)
{
    ros::init(argc, argv, "loa_node");

    LOA_CORE loa;

    loa.start();

    return 0;
}
#include <string>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>


int main( int argc, char *argv[]){

    const std::string robot_name = "/simulate_robot";
    const std::string publish_topic = "/cmd_vel";
    const std::string robot_controller_name = "/r2d2_diff_drive_controller";
    const double PUBLISH_FREQUENCE = 10; // publish cyclic messages with 10 Hz
    const unsigned int PUBLISHER_QUEUE_SIZE = 100;

    ros::init(argc, argv, "motion_publisher");
    ros::NodeHandle node_handle;

    geometry_msgs::Vector3 linear_vel, angular_vel;
    // check all configurable params loaded successfully
    bool no_param_loaded = false;
    no_param_loaded |= !node_handle.getParam(robot_name + "/geometry/linear_velocity_x", linear_vel.x);
    no_param_loaded |= !node_handle.getParam(robot_name + "/geometry/linear_velocity_y", linear_vel.y);
    no_param_loaded |= !node_handle.getParam(robot_name + "/geometry/linear_velocity_z", linear_vel.z);
    no_param_loaded |= !node_handle.getParam(robot_name + "/geometry/angular_velocity_x", angular_vel.x);
    no_param_loaded |= !node_handle.getParam(robot_name + "/geometry/angular_velocity_y", angular_vel.y);
    no_param_loaded |= !node_handle.getParam(robot_name + "/geometry/angular_velocity_z", angular_vel.z);
     
    // avoid unexpected behavior from not set (default) values during simulation.
    ROS_ASSERT_MSG(!no_param_loaded, "at least 1 motion config not loaded correctly, please check the motion.yaml file");
    ROS_INFO_STREAM("the linear velocity:\n" << linear_vel);
    ROS_INFO_STREAM("the angular velocity:\n" << angular_vel);

    // setup the publisher
    ros::Publisher motion_publisher = node_handle.advertise<geometry_msgs::Twist>(robot_controller_name + publish_topic, PUBLISHER_QUEUE_SIZE);

    // set the message to be sent
    geometry_msgs::Twist motion_msg;
    motion_msg.linear = linear_vel;
    motion_msg.angular = angular_vel;

    ros::Rate loop_rate(PUBLISH_FREQUENCE);

    while(ros::ok()){
        motion_publisher.publish(motion_msg);

        // to allow processing of incoming messages(callbacks)
        ros::spinOnce(); 

        // set the publish frequence    
        loop_rate.sleep();
    }

    return 0;
}
#include <iostream>
#include <ros/ros.h>
#include <kkctbn2019/Command.h>
#include <kkctbn2019/Mode.h>
#include <mavros_msgs/OverrideRCIn.h>
#include <mavros_msgs/SetMode.h>

ros::Publisher override_publisher;
kkctbn2019::Mode mode;

void commandCallback(const kkctbn2019::Command::ConstPtr& msg) {

    if (mode.value == 3) {
        ROS_INFO("MANUAL");
    } else if (mode.value == 1) {
        ROS_INFO("AUTO");
        if (msg->maju) {
            ROS_INFO("maju");
        } else if (msg->mundur) {
            ROS_INFO("mundur");
            mavros_msgs::OverrideRCIn rcin;
            for (int i = 0; i < 8; i ++) rcin.channels[i] = 0;
            rcin.channels[2] = 1800;
            override_publisher.publish(rcin);
        } else if (msg->kiri) {
            ROS_INFO("kiri");
        } else if (msg->kanan) {
            ROS_INFO("kanan");
        }
    } else {
        ROS_INFO("HOLD");
    }
}

void modeCallback(const kkctbn2019::Mode::ConstPtr& msg) {
    mode = *msg;
    if (mode.value == 3) {
        mavros_msgs::OverrideRCIn rcin;
        for (int i = 0; i < 8; i ++) rcin.channels[i] = 0;
        override_publisher.publish(rcin);
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "controller");
    ros::NodeHandle nh;

    override_publisher = nh.advertise<mavros_msgs::OverrideRCIn>("/mavros/rc/override", 10);

    ros::Subscriber mode_subscriber = nh.subscribe("/makarax/mode", 8, modeCallback);

    ros::Subscriber command_subscriber = nh.subscribe("/makarax/command", 8, commandCallback);

    ROS_WARN("controller is active");

    ros::spin();
    
    return 0;
}



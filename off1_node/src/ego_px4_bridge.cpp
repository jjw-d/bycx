#include <ros/ros.h>
#include <quadrotor_msgs/PositionCommand.h>

#include <geometry_msgs/PoseStamped.h>


ros::Publisher pub;

void cmd_callback(const quadrotor_msgs::PositionCommand::ConstPtr& msg) {
    geometry_msgs::PoseStamped target_pose;
    
    // 填充标准头部信息
    target_pose.header.stamp = ros::Time::now();
    target_pose.header.frame_id = "world"; // 确保与你的定位坐标系一致

    // 坐标转换：将规划器的位置传给 MAVROS
    target_pose.pose.position.x = msg->position.x;
    target_pose.pose.position.y = msg->position.y;
    target_pose.pose.position.z = msg->position.z;

    // 偏航角转换（如果规划器输出了偏航角）
    // 注意：PX4使用四元数，此处简单处理或保持默认
    target_pose.pose.orientation.w = 1.0; 

    pub.publish(target_pose);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "cmd_transfer_node");
    ros::NodeHandle nh;

    // 订阅 Ego-Planner 的输出话题
    ros::Subscriber sub = nh.subscribe<quadrotor_msgs::PositionCommand>("/planning/pos_cmd", 10, cmd_callback);
    
    // 发布给 MAVROS 的输入话题
    pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);

    ros::spin();
    return 0;
}
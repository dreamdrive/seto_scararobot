// 20191212 kawata氏作成
// 20191215 Micchy修正
// Pointをsubscriveして、jointstateをpublish
//
// move_test2
//

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Point.h>
#include <string>
#include <math.h>
#include <std_msgs/String.h>

// アームの長さを入れる(単位はビーズを置く位置（beads.x ,beads.y）と揃える)
// 現在はmm

float arm1_length = 90.0;
float arm2_length = 90.0;

float arm1_sita = 0.0;
float arm2_sita = 0.0;
float beads_pos_x = 0.0;
float beads_pos_y = 0.0;

const std::string MSG_ARM_WAITING    = "Waiting";
const std::string MSG_ARM_MOVING    = "Moving";
const std::string MSG_ARM_GOAL    = "Goal";

void armPositionCallback(const geometry_msgs::Point &beads)
{
	beads_pos_x = (float)beads.x;
	beads_pos_y = (float)beads.y;
}

void calculate_arm_pos()
{
  float arm1_length_2 = arm1_length * arm1_length;
  float arm2_length_2 = arm2_length * arm2_length;
  float x = beads_pos_x;
  float y = beads_pos_y;
  float x_2 = x * x;
  float y_2 = y * y;

  // 20191215 Micchy追加
  float length_goal;
  length_goal =  sqrt(x_2 + y_2);

	if ( ((arm1_length + arm2_length) <= length_goal)  || (length_goal == 0.0)){
    arm1_sita = 0.0;
    arm2_sita = 0.0;
    ROS_INFO("arm1_sita is %lf",arm1_sita);
    ROS_INFO("arm2_sita is %lf",arm2_sita);
    return;
	}

  // 20191215 Micchy追加
  // 20200317 更新
  if (y > 0){
    arm1_sita = atan2(y,x) - acosf((length_goal/2) / arm1_length);
    arm2_sita = -2 * (asinf((length_goal/2) / arm1_length));
  }
  else{
    arm1_sita = atan2(y,x) + acosf((length_goal/2) / arm1_length);
    arm2_sita = 2 * (asinf((length_goal/2) / arm1_length));
  }
  ROS_INFO("arm1_sita is %lf",arm1_sita);
  ROS_INFO("arm2_sita is %lf",arm2_sita);
  return;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "move_arm");  // ノードの初期化
  ros::NodeHandle nh; // ノードハンドラ

  //パブリッシャの作成
  ros::Publisher pub_scara_arm;
  ros::Publisher pub_arm_state;
  ros::Subscriber sub_beads;
  
  pub_scara_arm = nh.advertise<sensor_msgs::JointState>("/joint_states",1);
  pub_arm_state = nh.advertise<std_msgs::String>("/arm_states",1);
  sub_beads = nh.subscribe("/arm_positions", 60, armPositionCallback);
  
  ros::Rate loop_rate(60);  // 制御周期60Hz

  sensor_msgs::JointState scara_arm;
  std_msgs::String arm_state;

  scara_arm.name.resize(3);
  scara_arm.name[0] = "base_to_arm1";
  scara_arm.name[1] = "arm1_to_arm2";
  scara_arm.name[2] = "end_joint";

  scara_arm.position.resize(3);
  scara_arm.position[0] = 0.0;
  scara_arm.position[1] = 0.0;
  scara_arm.position[2] = 0.0;

  while(ros::ok()){
    scara_arm.header.stamp = ros::Time::now();
    calculate_arm_pos();
    scara_arm.position[0] = arm1_sita;
    scara_arm.position[1] = arm2_sita;
    scara_arm.position[2] = 0.0;
    pub_scara_arm.publish(scara_arm);
    ros::spinOnce();   // コールバック関数を呼ぶ
    loop_rate.sleep();
  }
  return 0;
}


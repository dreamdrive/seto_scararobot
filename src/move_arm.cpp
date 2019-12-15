// 20191212 kawata氏作成
// 20191215 Micchy修正

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Point.h>
#include <string>
#include <math.h>

// アームの長さを入れる(単位はビーズを置く位置（beads.x ,beads.y）と揃える)
// 現在はmm

float arm1_length = 90.0;
float arm2_length = 90.0;

float arm1_sita = 0.0;
float arm2_sita = 0.0;
float beads_pos_x = 0.0;
float beads_pos_y = 0.0;

void beadsCallback(const geometry_msgs::Point &beads)
{
	beads_pos_x = (float)beads.x;
	beads_pos_y = (float)beads.y;
    //ROS_INFO("%lf",beads_pos_x);
    //ROS_INFO("%lf",beads_pos_y);
}
/*
void init()
{
	scara_arm.name.resize(2);
	scara_arm.name[0] = "base_to_arm1";
	scara_arm.name[1] = "arm1_to_arm2";
	scara_arm.position.resize(2);
	scara_arm.header.stamp = ros::Time::now();
	scara_arm.position[0] = 0.0;
	scara_arm.position[1] = 0.0;
}
*/

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
  float sita1,sita2;
  length_goal =  sqrt(x_2 + y_2);

	if ( ((arm1_length + arm2_length) <= length_goal)  || ((x_2 + y_2) == 0.0)){
        arm1_sita = 0.0;
         arm2_sita = 0.0;
         ROS_INFO("arm1_sita is %lf",arm1_sita);
         ROS_INFO("arm2_sita is %lf",arm2_sita);
         return;
	  }

  // 20191215 Micchy追加
  sita1 = atan2(y,x);
  sita2 = acosf((length_goal/2) / arm1_length);
  arm1_sita = sita1 + sita2;
  arm2_sita = 2 * (asinf((length_goal/2) / arm1_length));

  //   if (y >= 0){
	//     arm1_sita = acosf( (x_2 + y_2 + arm1_length_2 - arm2_length_2) / (2 * arm1_length * sqrt((x_2 + y_2))) ) + atan2(y,x);
  //     arm2_sita = atan2( (y - arm1_length * sin(arm1_sita)), (x - (arm1_length * cos(arm1_sita))) ) - arm1_sita;
  //   }
  //   else if (y < 0){
  //     arm1_sita = acosf( ((x_2 + y_2 + arm1_length_2 - arm2_length_2) / (2 * arm1_length * sqrt((x_2 + y_2)) ))) - atan2(y,x);
  //     arm2_sita = -atan2( (y - arm1_length * sin(arm1_sita)), (x - (arm1_length * cos(arm1_sita))) ) - arm1_sita;

  //     //arm1_sita *= -1.0;
  //     arm2_sita *= -1.0;
  //   }

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
  ros::Subscriber sub_beads;
  pub_scara_arm = nh.advertise<sensor_msgs::JointState>("/joint_states",1);
  sub_beads = nh.subscribe("/beads_position", 60, beadsCallback);
  ros::Rate loop_rate(60);  // 制御周期60Hz
  sensor_msgs::JointState scara_arm;
  scara_arm.name.resize(2);
  scara_arm.name[0] = "base_to_arm1";
  scara_arm.name[1] = "arm1_to_arm2";
  scara_arm.position.resize(2);
  scara_arm.position[0] = 0.0;
  scara_arm.position[1] = 0.0;
  ROS_INFO("seto_scararobot start!");

  while(ros::ok()){
    scara_arm.header.stamp = ros::Time::now();
    calculate_arm_pos();
    scara_arm.position[0] = arm1_sita;
    scara_arm.position[1] = arm2_sita;
    pub_scara_arm.publish(scara_arm);
    ros::spinOnce();   // コールバック関数を呼ぶ
    loop_rate.sleep();
  }
  return 0;
}


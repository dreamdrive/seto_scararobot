// 20191212 kawata氏作成
// 20191215 Micchy修正
// Pointをsubscriveして、joint_trajectryをpublish
// 20200317 Micchy 逆運動学でバグ(x<0,y>0の領域)があったので修正

#include "ros/ros.h"
#include "ros/time.h"

#include <sensor_msgs/JointState.h>
#include "trajectory_msgs/JointTrajectory.h"
#include <geometry_msgs/Point.h>

#include "std_msgs/Float64.h"
#include "std_msgs/String.h"

#include <math.h>

// アームの長さを入れる(単位はビーズを置く位置（beads.x ,beads.y）と揃える)
// 現在はmm

float arm1_length = 90.0;
float arm2_length = 90.0;

float arm1_sita = 0.0;
float arm2_sita = 0.0;
float beads_pos_x = 0.0;
float beads_pos_y = 0.0;

// 現在の状態を保存する変数
std_msgs::String joint_name[2];
std_msgs::Float64 joint_pos[2];
std_msgs::Float64 joint_vel[2];
std_msgs::Float64 joint_eff[2];

void beadsCallback(const geometry_msgs::Point &beads)
{
	beads_pos_x = (float)beads.x;
	beads_pos_y = (float)beads.y;
    //ROS_INFO("%lf",beads_pos_x);
    //ROS_INFO("%lf",beads_pos_y);
}

void calculate_arm_pos(float x,float y)
{
  float arm1_length_2 = arm1_length * arm1_length;
  float arm2_length_2 = arm2_length * arm2_length;
//  float x = beads_pos_x;
//  float y = beads_pos_y;
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
  ros::init(argc, argv, "move_arm_dynamixel");  // ノードの初期化
  ros::NodeHandle nh; // ノードハンドラ

  float tmp_beads_pos_x , tmp_beads_pos_y ,old_beads_pos_x,old_beads_pos_y,old_arm1_sita,old_arm2_sita;

  //パブリッシャの作成
  ros::Publisher pub_scara_arm_goal;
  pub_scara_arm_goal = nh.advertise<sensor_msgs::JointState>("/seto_scararobot/goal",1);

  //パブリッシャの作成
  ros::Publisher pub_scara_arm_trajectory;
  pub_scara_arm_trajectory = nh.advertise<trajectory_msgs::JointTrajectory>("/dynamixel_workbench/joint_trajectory",1);

  //サブスクライバの作成
  ros::Subscriber sub_beads;
  sub_beads = nh.subscribe("/beads_position", 60, beadsCallback);

  ros::Rate loop_rate(60);  // 制御周期60Hz

  // 目標(JointState)を生成
  sensor_msgs::JointState scara_arm;
  scara_arm.name.resize(3);
  scara_arm.name[0] = "base_to_arm1";
  scara_arm.name[1] = "arm1_to_arm2";
  scara_arm.name[2] = "end_joint";
  scara_arm.position.resize(3);
  scara_arm.position[0] = 0.0;
  scara_arm.position[1] = 0.0;
  scara_arm.position[2] = 0.0;

  // ポーズ(JointTrajectory)を生成
  trajectory_msgs::JointTrajectory jtp0;
  jtp0.header.frame_id = "base_link";             // ポーズ名（モーション名)
  jtp0.joint_names.resize(2);                     // 関節名をセット
  jtp0.joint_names[0] ="base_to_arm1";
  jtp0.joint_names[1] ="arm1_to_arm2";
  jtp0.points.resize(2);                          // ポーズは2つ
  jtp0.points[0].positions.resize(2);             // ポーズ→positionsを2個設定  
  jtp0.points[0].velocities.resize(2);            // ポーズ→velocitiesを2個設定  
  jtp0.points[0].accelerations.resize(2);         // ポーズ→accelerationsを2個設定  
  jtp0.points[0].effort.resize(2);                // ポーズ→effortを2個設定  
  jtp0.points[1].positions.resize(2);             // ポーズ→positionsを2個設定  
  jtp0.points[1].velocities.resize(2);            // ポーズ→velocitiesを2個設定  
  jtp0.points[1].accelerations.resize(2);         // ポーズ→accelerationsを2個設定  
  jtp0.points[1].effort.resize(2);                // ポーズ→effortを2個設定  

  // 原点ポーズをセット
  jtp0.points[0].positions[0] = 0.0;
  jtp0.points[0].positions[1] = 0.0;
  jtp0.points[0].time_from_start = ros::Duration(0.0);  //実行時間0.0sec
  jtp0.points[1].positions[0] = 0.0;
  jtp0.points[1].positions[1] = 0.0;
  jtp0.points[1].time_from_start = ros::Duration(1.0);  //実行時間1.0sec
  old_arm1_sita = 0.0;
  old_arm2_sita = 0.0;

  ROS_INFO("seto_scararobot start!");

  while(ros::ok()){

    // ----変更があったときだけpublish -----------------------
    tmp_beads_pos_x = beads_pos_x;
    tmp_beads_pos_y = beads_pos_y;
    if ((tmp_beads_pos_x != old_beads_pos_x)||(tmp_beads_pos_y != old_beads_pos_y)){
      jtp0.header.stamp = ros::Time::now();
      scara_arm.header.stamp = ros::Time::now();

      calculate_arm_pos(beads_pos_x,beads_pos_y);
      jtp0.points[0].positions[0] = old_arm1_sita;
      jtp0.points[0].positions[1] = old_arm2_sita;
      jtp0.points[1].positions[0] = arm1_sita;
      jtp0.points[1].positions[1] = arm2_sita;

      scara_arm.position[0] = arm1_sita;
      scara_arm.position[1] = arm2_sita;
      scara_arm.position[2] = 0.0;

      //パブリッシュ (joint_trajectry)
      pub_scara_arm_trajectory.publish(jtp0);
      pub_scara_arm_goal.publish(scara_arm);
    }
    old_beads_pos_x = tmp_beads_pos_x;
    old_beads_pos_y = tmp_beads_pos_y;
    old_arm1_sita = arm1_sita;
    old_arm2_sita = arm2_sita;

    // ----変更があったときだけpublish -----------------------

    ros::spinOnce();   // コールバック関数を呼ぶ
    loop_rate.sleep();

  }
  return 0;
}


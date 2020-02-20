//
// moveitのjoint_statesをSubscribeして、1ポーズのjoint_trajectoryに変換
// "/dynamixel_workbench/joint_trajectory"にPublish
// って感じの強引なノード(joint_stateを反映する)
//

#include "ros/ros.h"
#include "ros/time.h"

#include "std_msgs/Float64.h"
#include "std_msgs/String.h"

#include "trajectory_msgs/JointTrajectory.h"
#include "sensor_msgs/JointState.h"

std_msgs::String joint_name[2];
std_msgs::Float64 joint_pos[2];
std_msgs::Float64 joint_vel[2];
std_msgs::Float64 joint_eff[2];

//コールバックがあるとグローバルに読み込み
void monitorJointState_callback(const sensor_msgs::JointState::ConstPtr& jointstate)
{
  int i,j;
  for(i=0;i<2;i++){
    for(j=0;j<2;j++){
      if(joint_name[i].data == jointstate->name[j]){
        joint_pos[i].data = jointstate->position[j];    // ポジション読み出し
       // joint_vel[i].data = jointstate->velocity[j];  // 速度読み出し
       // joint_eff[i].data = jointstate->effort[j];    // 負荷読み出し
      }
    }
  }
}

int main(int argc, char **argv)
{
  int i;
  ros::init(argc, argv, "moveit2dynamixel");      // ノードの初期化
  ros::NodeHandle nh;                             // ノードハンドラ

  ros::Subscriber sub_joints;                     // サブスクライバの作成
  sub_joints = nh.subscribe("/joint_states", 1, monitorJointState_callback);    // moveit_demoが吐き出すjoint_stateから抜き取る

  ros::Publisher arm_pub;                         //パブリッシャの作成
  arm_pub = nh.advertise<trajectory_msgs::JointTrajectory>("/dynamixel_workbench/joint_trajectory",1);

  ros::Rate loop_rate(10);                        // 制御周期45Hz(サーボの更新が20msecなのですが、50Hzだと余裕ない感じ)

  trajectory_msgs::JointTrajectory jtp0;
  
  jtp0.header.frame_id = "base_link";             // ポーズ名（モーション名)

  jtp0.joint_names.resize(2);                     // 名前
  jtp0.joint_names[0] ="base_to_arm1";
  jtp0.joint_names[1] ="arm1_to_arm2";

  joint_name[0].data = "base_to_arm1";
  joint_name[1].data = "arm1_to_arm2";

  jtp0.points.resize(1);                          // ポーズは一つだけ

  jtp0.points[0].positions.resize(2);             // positionsを2個設定  
  jtp0.points[0].velocities.resize(2);            // velocitiesを2個設定  
  jtp0.points[0].accelerations.resize(2);         // accelerationsを2個設定  
  jtp0.points[0].effort.resize(2);                // effortを2個設定  

  for(i=0;i<2;i++){
    jtp0.points[0].positions[i] = 0.0;
  }

  jtp0.points[0].time_from_start = ros::Duration(0.1);  //実行時間20msec (10msecだと動作しない;;)

  while (ros::ok()) { // 無限ループ

    jtp0.header.stamp = ros::Time::now();
    
    jtp0.points[0].positions[0] = joint_pos[0].data;  // 右から右
    jtp0.points[0].positions[1] = joint_pos[1].data;  // 右から右

    arm_pub.publish(jtp0);

    ros::spinOnce();    // コールバック関数を呼ぶ
    loop_rate.sleep();  // 待ち
  }
  
  return 0;
}

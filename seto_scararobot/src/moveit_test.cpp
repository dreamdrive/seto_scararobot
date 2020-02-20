//
// moveitで目標指定して実行するテストファイル
// 動かないのですが誰か助けて〜 (>o<) 2019/12/30
//
// [ INFO] : ABORTED: No motion plan found. No execution attempted.
// [ WARN] : Could not move to ZAHYO! [ERROR]
// ↑という感じで、解が出ません。
//

#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "pickandplacer");
  ros::NodeHandle nh;

  ros::AsyncSpinner spinner(2);
  spinner.start();

  moveit::planning_interface::MoveGroupInterface arm("arm");

  ROS_INFO("Moving to prepare pose");
  ROS_INFO("Reference frame: %s", arm.getPlanningFrame().c_str());
  ROS_INFO("End effector link: %s", arm.getEndEffectorLink().c_str());

  arm.setPlanningTime(5.0);               // プランニングタイム 5.0sec以内に解がでなければタイムアウト
  arm.setPlannerId("RRTConnect");         // プランナー
  arm.setGoalTolerance(0.05);             // 許容誤差
  arm.setGoalPositionTolerance (0.05);    // 許容誤差
  arm.setGoalOrientationTolerance(0.05);  // 許容誤差

  ros::Duration(0.5).sleep();

  //ポーズ "default"(原点) に移動する    これは実行可能 ☆☆☆
  ROS_INFO("Moving to default pose[START]");
  arm.setNamedTarget("default");
  if (!arm.move()) {
    ROS_WARN("Could not move to prepare pose");
    return 1;
  }
  ROS_INFO("Moved to default pose [END]");

  ros::Duration(0.5).sleep();

  //ポーズ "test" に移動する    これは実行可能 ☆☆☆
  ROS_INFO("Moving to test pose[START]");
  arm.setNamedTarget("test");
  if (!arm.move()) {
    ROS_WARN("Could not move to prepare pose");
    return 1;
  }
  ROS_INFO("Moved to test pose [END]");

  ros::Duration(0.5).sleep();

  // X,Y,Z = (0,0,0)の座標に移動　　　これが実行出来ない ★★★
  ROS_INFO("Moving to ZAHYO! [START]");
  geometry_msgs::PoseStamped pose;
  pose.header.frame_id = "base_link";
  pose.pose.position.x = 0.00;
  pose.pose.position.y = 0.00;
  pose.pose.position.z = 0.216;
//  pose.pose.orientation.x = 0.0;
//  pose.pose.orientation.y = 0.0;
//  pose.pose.orientation.z = 0.0;
  pose.pose.orientation.w = 1.0;
  arm.setPoseTarget(pose);
  if (!arm.move()) {
      ROS_WARN("Could not move to ZAHYO! [ERROR]");
      return 1;
  }
  ROS_INFO("Moved to ZAHYO! [END]");

  ros::Duration(0.5).sleep();

  // X,Y,Z = (0.10,0,0)の座標に移動 (そもそもここまで到達しない) (TT)
  ROS_INFO("Moving to ZAHYO2! [START]");
  pose.pose.position.x = 0.10;
  arm.setPoseTarget(pose);
  if (!arm.move()) {
      ROS_WARN("Could not move to ZAHYO2! [ERROR]");
      return 1;
  }
  ROS_INFO("Moved to ZAHYO2! [END]");

  ros::shutdown();
  return 0;
}
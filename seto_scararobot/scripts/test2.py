#!/usr/bin/env python
import sys
import copy
import rospy
import moveit_commander
import moveit_msgs.msg
import geometry_msgs.msg
from math import pi
from std_msgs.msg import String
from moveit_commander.conversions import pose_to_list

rospy.init_node('move_group_client')
robot = moveit_commander.RobotCommander()
scene = moveit_commander.PlanningSceneInterface()
group_name = "arm"
group = moveit_commander.MoveGroupCommander(group_name)
display_trajectory_publisher = rospy.Publisher('/move_group/display_planned_path',
moveit_msgs.msg.DisplayTrajectory,
queue_size=20)

# We can get the name of the reference frame for this robot:
planning_frame = group.get_planning_frame()
print "============ Reference frame: %s" % planning_frame

# We can also print the name of the end-effector link for this group:
eef_link = group.get_end_effector_link()
print "============ End effector: %s" % eef_link

# We can get a list of all the groups in the robot:
group_names = robot.get_group_names()
print "============ Robot Groups:", robot.get_group_names()

# Sometimes for debugging it is useful to print the entire state of the
# robot:
print "============ Printing robot state"
print robot.get_current_state()
print ""

# We can get the joint values from the group and adjust some of the values:
joint_goal = group.get_current_joint_values()

pose_goal = geometry_msgs.msg.Pose()
pose_goal.position.x = 0.040995
pose_goal.position.y = -0.123131
pose_goal.position.z = 0.216
pose_goal.orientation.x = 0.0
pose_goal.orientation.y = 0.0
pose_goal.orientation.z = 0.517125
pose_goal.orientation.w = 0.85591

group.set_planning_time(10)
group.set_goal_tolerance(0.1)
group.set_pose_target(pose_goal)

#group.go(joint_goal, wait=True)
group.go(wait=True)

# Calling ``stop()`` ensures that there is no residual movement
group.stop()
group.clear_pose_targets()

#!/usr/bin/env bash

#publish via command line
rostopic pub -r 10 /r2d2_diff_drive_controller/cmd_vel geometry_msgs/Twist  '{linear:  {x: 0.1, y: 0.0, z: 0.0}, angular: {x: 0.0,y: 0.0,z: 0.5}}'